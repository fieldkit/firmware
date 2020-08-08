#!/usr/bin/python3

import binascii
import struct
import zlib


def from_number(use, key):
    animal_index = key % len(animals)
    adjective_index = key % len(adjectives)
    suffix = key % 100

    adjective = adjectives[adjective_index]
    animal = animals[animal_index]

    uses[animal][use] += 1
    uses[adjective][use] += 1

    return "{} {} {}".format(adjective, animal, suffix)


def old_name(id):
    data = struct.unpack("iiii", id)
    key = data[0] + data[1] + data[2] + data[3]
    return from_number(0, key)


def crc32_name(id):
    key = zlib.crc32(id)
    return from_number(1, key)


incoming_ids = open("device-ids.txt", "r").readlines()
adjectives = open("../fk/data/adjectives.txt", "r").readlines()
animals = open("../fk/data/animals.txt", "r").readlines()

adjectives = list(map(lambda s: s.strip(), adjectives))
animals = list(map(lambda s: s.strip(), animals))

uses = {}

for word in adjectives:
    uses[word] = [0, 0]

for word in animals:
    uses[word] = [0, 0]


for id in map(lambda s: s.strip(), incoming_ids):
    if len(id) == 32:
        idBytes = binascii.unhexlify(id)
        cols = [id, old_name(idBytes), crc32_name(idBytes)]
        print(",".join(cols))

for word in animals:
    if sum(uses[word]) > 0:
        print("{},{}".format(word, ",".join(map(str, uses[word]))))
