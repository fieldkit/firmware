#!/usr/bin/python3

import subprocess
import re
import sys
import logging
import cxxfilt

from dataclasses import dataclass, field
from typing import Optional


@dataclass
class Symbol:
    address: int
    name: str


if __name__ == "__main__":
    logging.basicConfig(
        level=logging.INFO,
        format="[%(levelname)5s] %(message)s",
        handlers=[
            logging.StreamHandler(sys.stdout),
        ],
    )

    log = logging.getLogger("symbols")

    path = "/home/jlewallen/downloads/fk-firmware-0.1.3-develop.157-2a216c3f/fk-bundled-fkb.elf"
    sp = subprocess.run(
        f"readelf -s {path}",
        shell=True,
        stdout=subprocess.PIPE,
    )

    symbols = []

    def find_symbol(needle: int) -> Optional[Symbol]:
        if needle > 0x8000 and needle < symbols[-1].address:
            previous = None
            for symbol in symbols:
                log.debug(f"checking {needle:08x} {needle} {symbol}")
                if previous and symbol.address >= needle:
                    return previous
                previous = symbol
        else:
            log.debug(f"out of range: {needle}")
        return None

    skipping = 2
    for line in sp.stdout.strip().decode("utf-8").split("\n"):
        if skipping > 0:
            skipping -= 1
            continue
        fields = re.split("\s+", line.strip())
        address = int("0x" + fields[1], 0)
        if len(fields) != 8 or fields[3] != "FUNC":
            log.debug(f"ignoring {fields}")
            continue
        if address % 2 == 1:
            address -= 1
        name = fields[7]
        try:
            name = cxxfilt.demangle(fields[7])
        except:
            pass
        symbol = Symbol(address, name)
        symbols.append(symbol)

    symbols.sort(key=lambda x: x.address)

    for symbol in symbols:
        log.debug(f"symbol: {symbol}")

    if False:
        s = find_symbol(221449)
        log.info(f"{s}")
        assert s
        sys.exit(0)

    for line in [l.strip() for l in sys.stdin]:
        if "coredump" in line:
            log.info(line)

        if not line.startswith("stack: "):
            continue

        if "(*)" in line:
            continue

        fields = re.split("\s+", line)
        log.debug(line)

        for stack_item in [f for f in fields[1:]]:
            needle = int("0x" + stack_item, 0) + 1

            symbol = find_symbol(needle)
            if symbol:
                log.info(f"found {needle:08x} {needle} {symbol}")
