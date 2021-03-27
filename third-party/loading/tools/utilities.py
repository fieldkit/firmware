from typing import Any, Optional

import logging
import lief  # type: ignore
import pyblake2  # type: ignore

from elftools.elf.elffile import ELFFile  # type: ignore
from elftools.elf.relocation import RelocationSection  # type: ignore
from elftools.elf.sections import SymbolTableSection  # type: ignore
from elftools.elf.relocation import RelocationSection  # type: ignore
from elftools.elf.descriptions import describe_reloc_type  # type: ignore


class Symbol:
    def __init__(self, section: Any, index: int, s: Any):
        self.name = s.name
        sbind = s["st_info"]["bind"]
        if sbind == "STB_GLOBAL":
            self.binding = lief.ELF.SYMBOL_BINDINGS.GLOBAL
        elif sbind == "STB_LOCAL":
            self.binding = lief.ELF.SYMBOL_BINDINGS.LOCAL
        elif sbind == "STB_WEAK":
            self.binding = lief.ELF.SYMBOL_BINDINGS.WEAK
        else:
            raise Exception("Unknown bind: '%s'" % (sbind))
        stype = s["st_info"]["type"]

        if stype == "STT_FILE":
            self.type = lief.ELF.SYMBOL_TYPES.FILE
        elif stype == "STT_OBJECT":
            self.type = lief.ELF.SYMBOL_TYPES.OBJECT
        elif stype == "STT_FUNC":
            self.type = lief.ELF.SYMBOL_TYPES.FUNC
        elif stype == "STT_NOTYPE":
            self.type = None
        elif stype == "STT_SECTION":
            self.type = lief.ELF.SYMBOL_TYPES.SECTION
        else:
            raise Exception("Unknown stype: '%s'" % (stype))

        self.index = index
        self.size = int(s["st_size"])
        self.shndx = s["st_shndx"]
        try:
            self.shndx = int(s["section"])
            self.exported = s["section"] != "SHN_UNDEF"
        except:
            self.exported = False
        self.value = int(s["st_value"])


class Section:
    def __init__(self, s: Any):
        self.name: str = s.name
        self.virtual_address: int = int(s["sh_addr"])


class Relocation:
    def __init__(self, elf: Any, section: Any, symtable: Any, symbol_objects: Any, r):
        self.offset = int(r["r_offset"])
        stype = describe_reloc_type(r["r_info_type"], elf)
        if stype == "R_ARM_GOT_BREL":
            self.type = lief.ELF.RELOCATION_ARM.GOT_BREL
        elif stype == "R_ARM_ABS32":
            self.type = lief.ELF.RELOCATION_ARM.ABS32
        elif stype == "R_ARM_THM_CALL":
            self.type = lief.ELF.RELOCATION_ARM.THM_CALL
        elif stype == "R_ARM_THM_JUMP24":
            self.type = lief.ELF.RELOCATION_ARM.THM_JUMP24
        elif stype == "R_ARM_TARGET1":
            self.type = lief.ELF.RELOCATION_ARM.TARGET1
        elif stype == "R_ARM_PREL31":
            self.type = lief.ELF.RELOCATION_ARM.PREL31
        elif stype == "R_ARM_REL32":
            self.type = lief.ELF.RELOCATION_ARM.REL32
        elif stype == "R_ARM_JUMP_SLOT":
            self.type = lief.ELF.RELOCATION_ARM.JUMP_SLOT
        elif stype == "R_ARM_GLOB_DAT":
            self.type = lief.ELF.RELOCATION_ARM.GLOB_DAT
        else:
            raise Exception("Unknown rel type: '%s'" % (stype))

        symbol_index = r["r_info_sym"]
        s = symtable.get_symbol(symbol_index)
        if s["st_name"] != 0:
            self.name = str(s.name)
            self.symbol = symbol_objects[symbol_index]
            self.has_symbol = True
        else:
            self.symbol = None
            self.has_symbol = False

        try:
            symsec = elf.get_section(s["st_shndx"])
            self.section = Section(symsec)
            self.name = self.section.name
        except:
            self.section = Section(section)
            self.name = self.section.name

        self.debug = "debug_" in self.section.name

        self.value = s["st_value"]
        self.address = s["st_value"]

    def __str__(self) -> str:
        if self.has_symbol:
            return "Relocation<%s type=%s address=0x%x value=0x%x offset=0x%x>" % (
                self.symbol.name,
                relocation_type_name(self.type),
                self.address,
                self.value,
                self.offset,
            )
        return "Relocation<%s type=%s address=0x%x value=0x%x offset=0x%x>" % (
            self.name,
            relocation_type_name(self.type),
            self.address,
            self.value,
            self.offset,
        )


# http://stackoverflow.com/a/4999321
class RejectingDict(dict):
    def __setitem__(self, k, v):
        if k in self.keys():
            if self[k] != v:
                raise ValueError(
                    "Key '%s' is already present value = (%s) new value = (%s)"
                    % (str(k), str(self[k]), str(v))
                )
        else:
            return super(RejectingDict, self).__setitem__(k, v)


def get_symbols_in_elf(path):
    symbols = []
    with open(path, "rb") as f:
        elf = ELFFile(f)
        for section in elf.iter_sections():
            if isinstance(section, SymbolTableSection):
                for i, symbol in enumerate(section.iter_symbols()):
                    symbols.append(Symbol(section, i, symbol))
    return symbols


def get_relocations_in_elf(path, symbol_objects):
    rels = []
    with open(path, "rb") as f:
        elf = ELFFile(f)
        for section in elf.iter_sections():
            if isinstance(section, RelocationSection):
                symtable = elf.get_section(section["sh_link"])
                for r in section.iter_relocations():
                    if r["r_info_sym"] == 0:
                        continue
                    rels.append(Relocation(elf, section, symtable, symbol_objects, r))
    return rels


def relocation_type_name(r):
    for attr, value in lief.ELF.RELOCATION_ARM.__dict__.items():
        if isinstance(value, lief.ELF.RELOCATION_ARM) and value == r:
            return attr
    return "<unknown>"


def append_hash(bin_path: str) -> int:
    logging.info("Calculating hash of '%s'" % (bin_path))

    b2 = pyblake2.blake2b(digest_size=32)
    with open(bin_path, "rb") as f:
        while True:
            data = f.read(65536)
            if not data:
                break
            b2.update(data)

    size = None

    with open(bin_path, "ab") as f:
        f.write(b2.digest())
        size = f.tell()

    with open(bin_path + ".b2sum", "w") as s:
        s.write(b2.hexdigest())

    logging.info("Hash '%s' (%d)" % (b2.hexdigest(), size))

    assert size
    return size


def aligned(size: int, on: int) -> int:
    if size % on != 0:
        return size + (on - (size % on))
    return size
