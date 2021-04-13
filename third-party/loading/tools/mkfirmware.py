#!/usr/bin/env python3

from typing import List, Union, Dict, Optional, Any

import os
import sys
import struct
import time
import datetime
import subprocess
import argparse
import hashlib
import logging
import lief  # type: ignore
import hashlib
import platform
import threading
import utilities
import pyblake2  # type: ignore

from collections import defaultdict


class TableRelocation:
    def __init__(self):
        super().__init__()


class TableSymbol:
    def __init__(
        self, name: str, size: int, got_offset: int, relocs: List[TableRelocation]
    ):
        super().__init__()
        self.name: str = name
        self.size: int = size
        self.got_offset: int = got_offset
        self.relocs: List[TableRelocation] = []


class ElfAnalyzer:
    def __init__(self, dynamic: bool, elf_path: str, increase_size_by: int):
        super().__init__()
        self.dynamic: bool = dynamic
        self.elf_path: str = elf_path
        self.increase_size_by: int = increase_size_by
        self.raw_cache: Dict[str, Any] = {}
        self.binary: Optional[lief.ELF.Binary] = None
        self.symbols: Dict[TableSymbol, TableSymbol] = {}
        self.relocations: List[TableRelocation] = []

    def timestamp(self) -> int:
        return int(os.path.getmtime(self.elf_path))

    def fkbheader(self) -> Optional[lief.ELF.Section]:
        assert self.binary
        try:
            return self.binary.get_section(".data.fkb.header")
        except:
            return None

    def fkdyn(self) -> Optional[lief.ELF.Section]:
        assert self.binary
        try:
            return self.binary.get_section(".fkdyn")
        except:
            return None

    def code(self) -> lief.ELF.Section:
        assert self.binary
        return self.binary.get_section(".text")

    def data(self) -> Optional[lief.ELF.Section]:
        assert self.binary
        try:
            return self.binary.get_section(".data")
        except:
            return None

    def got(self) -> Optional[lief.ELF.Section]:
        assert self.binary
        try:
            return self.binary.get_section(".got")
        except:
            return None

    def bss(self) -> Optional[lief.ELF.Section]:
        assert self.binary
        try:
            return self.binary.get_section(".bss")
        except:
            return None

    def by_name(self, name: str) -> Optional[lief.ELF.Section]:
        assert self.binary
        try:
            return self.binary.get_section(name)
        except:
            return None

    def get_got_size(self) -> int:
        g = self.got()
        if g is None:
            return 0
        return g.size

    def get_fkdyn_size(self) -> int:
        g = self.fkdyn()
        if g is None:
            return 0
        return g.size

    def get_bss_size(self) -> int:
        s = self.bss()
        if s is None:
            return 0
        return s.size

    def get_data_size(self) -> int:
        size = 0
        for section_name in [".data", ".data.rtt", ".data.fkb.header"]:
            section = self.by_name(section_name)
            if section:
                size += section.size
                logging.info(
                    "data-size: name=%s size=%d (0x%x) total=%d"
                    % (section.name, section.size, section.size, size)
                )
        return size

    def get_binary_size(self) -> int:
        size = 0
        for section in self.get_binary_sections():
            if section:
                size += section.size
                logging.info(
                    "bin-size: name=%s size=%d (0x%x) total=%d"
                    % (section.name, section.size, section.size, size)
                )
        if self.increase_size_by > 0:
            size += self.increase_size_by
            logging.info("bin-size: adding %d total=%d" % (self.increase_size_by, size))

        logging.info("bin-size: %d (0x%x)" % (size, size))
        return size

    def calculate_hash(self) -> bytes:
        algo = hashlib.sha1()
        code = self.code()
        if code:
            algo.update(bytearray(code.content))
        data = self.data()
        if data:
            algo.update(bytearray(data.content))
        return algo.digest()

    def get_code_address(self) -> int:
        s = self.code()
        assert s
        return s.virtual_address

    def raw_section_data(self, section):
        assert section
        if isinstance(section, utilities.Section):
            for s in self.binary.sections:
                if s.name == section.name:
                    return self.raw_section_data(s)
            raise Exception("Oops")
        if section.name in self.raw_cache.keys():
            return self.raw_cache[section.name]
        logging.info("Opening %s...", section.name)
        byte_data = bytearray(section.content)
        self.raw_cache[section.name] = byte_data
        logging.info("Processing %s (%d)", section.name, len(byte_data))
        return byte_data

    def get_relocations_in_binary(self):
        started = time.time()

        by_section_name_index = defaultdict(list)
        for r in self.binary.relocations:
            if r.has_section:
                by_section_name_index[r.section.name_idx].append(r)

        logging.info("Done %f", time.time() - started)

        relocations = []

        skipping = self.get_sections_to_skip()

        for s in self.binary.sections:
            if s.name not in skipping:
                relocations += by_section_name_index[s.name_idx]

        logging.info("Done %f", time.time() - started)

        for r in self.binary.relocations:
            if r.has_symbol:
                relocations.append(r)

        return relocations

    def get_sections_to_skip(self):
        return {
            ".debug_loc": 0,
            ".debug_frame": 0,
            ".debug_info": 0,
            ".debug_line": 0,
            ".debug_ranges": 0,
            ".debug_aranges": 0,
        }

    def got_origin(self):
        got = self.got()
        if got:
            return got.virtual_address
        return 0

    def find_relocations(self):
        skipping = self.get_sections_to_skip()
        symbols = utilities.RejectingDict()
        nsections = len(self.binary.sections)
        elf_symbols = utilities.get_symbols_in_elf(self.elf_path)

        logging.info(
            "Number of Symbols: %d (%d)" % (len(elf_symbols), len(self.binary.symbols))
        )

        for symbol in elf_symbols:
            if symbol.name in ["$t", "$d", ""]:
                continue

            if symbol.binding == lief.ELF.SYMBOL_BINDINGS.GLOBAL:
                if symbol.exported or symbol.shndx != 0:
                    symbols[symbol.index] = "exported"
                else:
                    symbols[symbol.index] = "external"
                    logging.info("External: %s", symbol.name)
            else:
                if symbol.type != lief.ELF.SYMBOL_TYPES.FILE:
                    symbols[symbol.index] = "local"

        logging.info(
            "Exported: %d", len([s for s in symbols if symbols[s] == "exported"])
        )

        logging.info(
            "External: %d", len([s for s in symbols if symbols[s] == "external"])
        )

        logging.info("Locals: %d", len([s for s in symbols if symbols[s] == "local"]))

        started = time.time()

        got_origin = self.got_origin()
        logging.info("GOT Origin: 0x%x" % (got_origin,))
        for r in self.get_relocations_in_binary():
            display = False
            rel_offset = 0
            fixed = None

            if r.type == lief.ELF.RELOCATION_ARM.GLOB_DAT and r.has_symbol:
                got_offset = r.address - got_origin
                logging.info(
                    "Relocation: sym=%s val=%s size=0x%x addr=0x%x offset=0x%x"
                    % (r.symbol.name, r.symbol.value, r.size, r.address, got_offset)
                )
                self.add_symbol(r.symbol, int(r.size / 8), r.address, got_offset)

        logging.info("Relocations Done")

    def add_symbol(self, symbol: lief.ELF.Symbol, size: int, address: int, offset: int):
        if not symbol in self.symbols:
            ts = TableSymbol(symbol.name, size, offset, [])
            self.symbols[symbol] = ts
        return self.symbols[symbol]

    def analyse(self):
        started = time.time()
        self.binary = lief.ELF.parse(self.elf_path)
        if self.dynamic:
            self.find_relocations()
        logging.info("Done, %s elapsed", time.time() - started)

    def get_binary_sections(self):
        return [
            self.by_name(".vectors"),
            self.fkbheader(),
            self.code(),
            self.by_name(".ARM.exidx"),
            self.by_name(".data.rtt"),
            self.data(),
            self.got(),
            self.fkdyn(),
        ]

    def write_bin(self, bin_path: str):
        with open(bin_path, "wb") as f:
            data = bytearray()
            for section in self.get_binary_sections():
                if section:
                    data += bytearray(section.content)
            f.write(data)

        expected_size = self.get_binary_size()
        actual_size = utilities.append_hash(bin_path)
        if actual_size != expected_size:
            logging.error(
                "Size mismatch: actual=%d vs expected=%d (actual - expected = %d)"
                % (actual_size, expected_size, actual_size - expected_size)
            )
        assert actual_size == expected_size


class FkbHeader:
    SIGNATURE_FIELD = 0
    HEADER_VERSION_FIELD = 1
    HEADER_SIZE_FIELD = 2
    FLAGS_FIELD = 3
    TIMESTAMP_FIELD = 4
    BUILD_NUMBER_FIELD = 5
    BINARY_SIZE_FIELD = 7
    TABLES_OFFSET_FIELD = 8
    BINARY_DATA_FIELD = 9
    BINARY_BSS_FIELD = 10
    BINARY_GOT_FIELD = 11
    VTOR_OFFSET_FIELD = 12
    GOT_OFFSET_FIELD = 13
    BINARY_VERSION_FIELD = 14
    HASH_SIZE_FIELD = 15
    HASH_FIELD = 16
    NUMBER_SYMBOLS_FIELD = 17
    NUMBER_RELOCATIONS_FIELD = 18

    def __init__(self, fkb_path: str, data: bytes):
        self.min_packspec: str = "<4sIIIII16sIIIIIII256sI128sII"
        self.min_size: int = struct.calcsize(self.min_packspec)
        self.fkb_path: str = fkb_path
        self.actual_size = len(data)
        self.extra = bytearray(data[self.min_size :])
        self.fields = list(
            struct.unpack(self.min_packspec, bytearray(data[: self.min_size]))
        )

    def has_invalid_version(self, value: str) -> bool:
        return len(value) == 0 or value[0] == "\0" or value[0] == 0

    def populate(self, ea: "ElfAnalyzer", name: str):
        self.fields[self.TIMESTAMP_FIELD] = ea.timestamp()
        self.fields[self.BINARY_SIZE_FIELD] = ea.get_binary_size()
        self.fields[self.TABLES_OFFSET_FIELD] = (
            ea.get_binary_size() - ea.get_fkdyn_size() - ea.increase_size_by
        )
        self.fields[self.BINARY_DATA_FIELD] = ea.get_data_size()
        self.fields[self.BINARY_BSS_FIELD] = ea.get_bss_size()
        self.fields[self.BINARY_GOT_FIELD] = ea.get_got_size()
        self.fields[self.VTOR_OFFSET_FIELD] = 1024

        got = ea.got()
        if got:
            self.fields[self.GOT_OFFSET_FIELD] = (
                got.virtual_address - 0x20000000
                if got.virtual_address > 0x20000000
                else got.virtual_address
            )
        else:
            self.fields[self.GOT_OFFSET_FIELD] = 0x0

        fwhash = ea.calculate_hash()
        self.fields[self.HASH_SIZE_FIELD] = len(fwhash)
        self.fields[self.HASH_FIELD] = fwhash

        if name:
            self.fields[self.BINARY_VERSION_FIELD] = name

        if self.has_invalid_version(self.fields[self.BINARY_VERSION_FIELD]):
            self.fields[self.BINARY_VERSION_FIELD] = self.generate_version(ea)

        if "BUILD_NUMBER" in os.environ:
            self.fields[self.BUILD_NUMBER_FIELD] = int(os.environ["BUILD_NUMBER"])

        self.fields[self.NUMBER_SYMBOLS_FIELD] = len(ea.symbols)
        self.fields[self.NUMBER_RELOCATIONS_FIELD] = len(ea.relocations)

    def generate_version(self, ea: ElfAnalyzer):
        name = os.path.basename(self.fkb_path)
        when = datetime.datetime.utcfromtimestamp(ea.timestamp())
        ft = when.strftime("%Y%m%d_%H%M%S")
        return bytes(name + "_" + platform.node() + "_" + ft, "utf8")

    def to_bytes(self):
        new_header = bytearray(bytes(struct.pack(self.min_packspec, *self.fields)))

        logging.info("Binary Version: %s" % (self.fields[self.BINARY_VERSION_FIELD]))
        logging.info("Header Version: %s" % (self.fields[self.HEADER_VERSION_FIELD]))
        logging.info("Number: %s" % (self.fields[self.BUILD_NUMBER_FIELD]))
        logging.info("Hash: %s" % (self.fields[self.HASH_FIELD].hex()))
        logging.info("Time: %d" % (self.fields[self.TIMESTAMP_FIELD]))
        logging.info("Binary size: %d bytes" % (self.fields[self.BINARY_SIZE_FIELD]))
        logging.info("GOT: 0x%x" % (self.fields[self.GOT_OFFSET_FIELD]))
        logging.info(
            "Header: %d bytes (%d of extra)" % (len(new_header), len(self.extra))
        )
        logging.info(
            "Dynamic: syms=%d rels=%d"
            % (
                self.fields[self.NUMBER_SYMBOLS_FIELD],
                self.fields[self.NUMBER_RELOCATIONS_FIELD],
            )
        )

        return new_header + self.extra


class FkbWriter:
    def __init__(self, elf_analyzer: ElfAnalyzer, fkb_path: str):
        self.ea: ElfAnalyzer = elf_analyzer
        self.fkb_path: str = fkb_path

    def populate_header_section(self, name: str):
        section = self.ea.fkbheader()
        if section is None:
            logging.info("No specialized handling for binary.")
            return
        logging.info("Found FKB section: %s bytes" % (section.size))
        header = FkbHeader(self.fkb_path, section.content)
        header.populate(self.ea, name)
        section.content = header.to_bytes()

    def generate(self, name: str):
        logging.info("Code Size: %d (%x)" % (self.ea.code().size, self.ea.code().size))
        logging.info("Data Size: %d" % (self.ea.get_data_size()))
        logging.info(" BSS Size: %d" % (self.ea.get_bss_size()))
        logging.info(" GOT Size: %d" % (self.ea.get_got_size()))
        if False:
            logging.info(" Dyn size: %d" % (self.table_size))

        self.generate_table()

        self.populate_header_section(name)

        assert self.ea.binary
        self.ea.binary.write(self.fkb_path)

    def generate_table(self):
        symbols = bytearray()
        relocations = bytearray()

        # Address in the symbol table we write to the image seems totally wrong...
        indices = {}
        index = 0
        for lief_symbol, table_symbol in self.ea.symbols.items():
            try:
                symbols += struct.pack(
                    "<24sII",
                    bytes(table_symbol.name, "utf-8"),
                    table_symbol.size,
                    table_symbol.got_offset,
                )
            except:
                raise Exception("Error packing symbol: %s" % (table_symbol,))

            indices[table_symbol] = index
            index += 1

        assert len(self.ea.relocations) == 0

        table_alignment = 4096
        self.table_size = utilities.aligned(
            len(symbols) + len(relocations), table_alignment
        )

        self.add_table_section(symbols + relocations, table_alignment)

        logging.info("Table size: %d" % (self.table_size,))

    def add_table_section(self, table: bytes, table_alignment: int):
        section = self.ea.fkdyn()
        if not section:
            return

        extra_padding = utilities.aligned(len(table), table_alignment) - len(table)
        section.content = table + bytearray([0] * extra_padding)

        logging.info(
            "Dynamic table virtual address: 0x%x table=%d size=%d padding=%d"
            % (len(table), section.virtual_address, len(section.content), extra_padding)
        )


def make_binary_from_elf(elf_path: str, bin_path: str, expected_size: int):
    command = ["arm-none-eabi-objcopy", "-O", "binary", elf_path, bin_path]
    logging.info("Exporting '%s' to '%s'" % (elf_path, bin_path))
    logging.info(" ".join(command))
    subprocess.run(command, check=True)

    actual_size = utilities.append_hash(bin_path)
    if actual_size != expected_size:
        logging.error(
            "Size mismatch: actual=%d vs expected=%d (actual - expected = %d)"
            % (actual_size, expected_size, actual_size - expected_size)
        )
    assert actual_size == expected_size


def configure_logging():
    if False:
        lief.Logger.enable()
        lief.Logger.set_level(lief.LOGGING_LEVEL.TRACE)
        lief.Logger.set_verbose_level(10)
    logging.basicConfig(format="%(asctime)-15s %(message)s", level=logging.INFO)


def main():
    configure_logging()

    parser = argparse.ArgumentParser(description="Firmware Preparation Tool")
    parser.add_argument(
        "--no-verbose",
        dest="no_verbose",
        action="store_true",
        help="Don't show verbose commands (default: false)",
    )
    parser.add_argument(
        "--no-debug",
        dest="no_debug",
        action="store_true",
        help="Don't show debug data (default: false)",
    )
    parser.add_argument(
        "--elf",
        dest="elf_path",
        default=None,
        help="Path to the compiler generated ELF file.",
    )
    parser.add_argument(
        "--fkb", dest="fkb_path", default=None, help="Path to the fkb ELF to generate."
    )
    parser.add_argument(
        "--bin",
        dest="bin_path",
        default=None,
        help="Path to the raw binary to generate. ",
    )
    parser.add_argument(
        "--name", dest="name", default=None, help="Override firmware name."
    )
    parser.add_argument(
        "--dynamic",
        dest="dynamic",
        default=False,
        action="store_true",
        help="Enable dynamic module mode. Relocations, the whole shebang.",
    )

    args, nargs = parser.parse_known_args()

    if args.elf_path:
        logging.info("Processing %s...", args.elf_path)

        ea: Optional[ElfAnalyzer] = None

        if args.fkb_path:
            increase_size_by = 32 if args.bin_path else 0

            ea = ElfAnalyzer(args.dynamic, args.elf_path, increase_size_by)
            ea.analyse()

            fw = FkbWriter(ea, args.fkb_path)
            fw.generate(args.name)

            if args.bin_path:
                if args.dynamic:
                    ea.write_bin(args.bin_path)
                else:
                    make_binary_from_elf(
                        args.fkb_path, args.bin_path, ea.get_binary_size()
                    )
        else:
            if args.bin_path:
                make_binary_from_elf(args.elf_path, args.bin_path)


if __name__ == "__main__":
    main()
