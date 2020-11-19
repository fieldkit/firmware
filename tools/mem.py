#!/usr/bin/python3

import argparse
import sys
import time
import logging
import os
import re
import fileinput


log = logging.getLogger("mem")


class Entry:
    def __init__(self, address=None, name=None, size=None, **kwargs):
        super().__init__()
        self.address = address
        self.name = name
        self.size = size

    def apply(self, memory):
        raise NotImplementedException


# 00018424 network    info    pool: create: 0x20017628 MetalNetworkConnection size=8192 ptr=0x2001764C
class Create(Entry):
    def __str__(self):
        return "create(%s %d bytes 0x%x)" % (self.name, self.size, self.address)

    def apply(self, memory):
        memory.mark(self.address, self.size, self.name)


# 00263247 readings   info    pool: destroy: 0x2001F628 readings size=8164 ptr=0x2001F644 (inside-pool;:delete)
# 00263249 readings   info    pool: destroy: 0x20017628 ReadingsWorker size=8156 ptr=0x2001764C (~standard-pool)
# 00004745 startup    info    pool: destroy: 0x20015628 StartupWorker size=8192 ptr=0x2001638C
class Destroy(Entry):
    def __str__(self):
        return "destroy(%s 0x%x + 0x%x)" % (self.name, self.address, self.size)

    def apply(self, memory):
        memory.unmark(self.address, self.size, self.name)


# 00018425 network    info    pool: malloc 0x20017628 MetalNetworkConnection size=8192 allocating=48/48 remaining=8108 remaining-aligned=8060 ptr=0x2001764C
class Malloc(Entry):
    def __str__(self):
        return "malloc(%d '%s') = 0x%x" % (
            self.size,
            self.name,
            self.address,
        )

    def apply(self, memory):
        pass


# 00006181 network    info    pool: clear: 0x20036DD0 network-tick size=8192
class Clear(Entry):
    def __str__(self):
        return "clear(%d %d '%s')" % (
            self.address,
            self.size,
            self.name,
        )

    def apply(self, memory):
        pass  # memory.free(self.address, self.size, self.name)


CreateRegex = re.compile(r"pool: create: \S+ (\S+) size=(\d+) ptr=(\S+)")
DestroyRegex = re.compile(r"pool: destroy: \S+ (\S+) size=(\d+) ptr=(\S+)")
MallocRegex = re.compile(
    r"pool: malloc (\S+) (\S+) size=(\d+) allocating=\d+/(\d+) remaining=\S+ remaining-aligned=\S+ ptr=(\S+)"
)
ClearRegex = re.compile(r"pool: clear: (\S+) (\S+) size=(\d+)")


def parse_number(s):
    return int(s, 0)


def make_entry(line):
    m = MallocRegex.search(line)
    if m:
        pool_address = parse_number(m[1])
        name = m[2]
        pool_size = parse_number(m[3])
        size = parse_number(m[4])
        address = parse_number(m[5])
        return Malloc(address=address, name=name, size=size, pool_address=pool_address)

    m = CreateRegex.search(line)
    if m:
        name = m[1]
        size = parse_number(m[2])
        address = parse_number(m[3])
        if False and name == "StartupWorker":
            log.info("{} hacked -= 24".format(name))
            size -= 0x24
        return Create(address=address, name=name, size=size)

    m = DestroyRegex.search(line)
    if m:
        name = m[1]
        size = parse_number(m[2])
        address = parse_number(m[3])
        return Destroy(address=address, name=name, size=size)

    m = ClearRegex.search(line)
    if m:
        address = parse_number(m[1])
        name = m[2]
        size = parse_number(m[3])
        return Clear(address=address, name=name, size=size)

    return None


class Block:
    def __init__(self, start: int, end: int, value: str = None):
        super().__init__()
        assert start != end
        assert start < end
        self.start = start
        self.end = end
        self.value = value

    def overlaps_with(self, other: "Block"):
        return self.start < other.end and other.start < self.end

    def same_as(self, other: "Block"):
        return self.start == other.start and self.end == other.end

    def __str__(self):
        return "block<0x%x -> 0x%x : %s>" % (self.start, self.end, self.value)


assert Block(0, 100).overlaps_with(Block(50, 120))
assert Block(99, 101).overlaps_with(Block(0, 110))
assert Block(0, 10).overlaps_with(Block(1, 10))
assert Block(0, 10).overlaps_with(Block(0, 1))
assert Block(0, 10).overlaps_with(Block(9, 10))
assert not Block(0, 100).overlaps_with(Block(100, 120))
assert not Block(0, 100).overlaps_with(Block(110, 120))
assert not Block(110, 120).overlaps_with(Block(0, 100))
assert not Block(100, 120).overlaps_with(Block(0, 100))


class Memory:
    def __init__(self):
        self.blocks = []

    def mark(self, address, size, value):
        marking = Block(address, address + size, value)

        for block in self.blocks:
            if block.overlaps_with(marking):
                raise Exception("overlap: {} vs {}".format(block, marking))

        log.info("marked {} {}".format(marking, size))

        self.blocks.append(marking)

    def unmark(self, address, size, value):
        unmarking = Block(address, address + size, value)
        for block in self.blocks[:]:
            if block.same_as(unmarking):
                self.blocks.remove(block)
                log.info("removing %s", block)
                return True

        if len(self.overlapping(unmarking)) == 0:
            if True:
                raise Exception("double free: {} {}".format(unmarking, size))
            log.warning("double free: {} {}".format(unmarking, size))
            return True

        log.error("{} {}".format(unmarking, size))
        raise Exception("unable to unmark")

    def overlapping(self, block):
        return [b for b in self.blocks if b.overlaps_with(block)]

    def dump(self):
        for b in self.blocks:
            log.info("{}".format(b))

    def clear(self):
        self.blocks = []


def verify_history(entries, memory):
    for entry in entries:
        log.info("{}".format(entry))
        entry.apply(memory)


def process(fp):
    entries = []
    memory = Memory()
    parsing = True
    for index, line in enumerate(fp):
        if parsing:
            if "flash: initialized" in line:
                memory.clear()
            entry = make_entry(line)
            if entry:
                # log.info("line : {}".format(line))
                log.info("entry: {} '{}'".format(entry, ""))
                entries.append(entry)
                try:
                    entry.apply(memory)
                except:
                    print()
                    log.error("line: {}".format(line))
                    log.error("line: {}".format(index))
                    raise

        if "raw log memory" in line:
            parsing = not parsing
            log.info("parsing: {}".format(parsing))

    log.info("MEMORY:")
    memory.dump()

    return entries, memory


def main():
    logging.basicConfig(stream=sys.stdout, level=logging.INFO)

    parser = argparse.ArgumentParser(description="memory verifier")
    parser.add_argument("file", nargs="*", help="log file")
    args, nargs = parser.parse_known_args()

    if len(nargs) == 0:
        process(fileinput.input())

    for file in args.file:
        logging.info(
            "opening {}".format(
                file,
            )
        )
        with open(file, "r") as fp:
            process(fp)


if __name__ == "__main__":
    main()
