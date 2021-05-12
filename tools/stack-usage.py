#!/usr/bin/python3

import os
import argparse


class Row:
    def __init__(self, fn, size):
        self.fn = fn
        self.size = size


def main():
    parser = argparse.ArgumentParser(description="Aggregate *.su files")
    parser.add_argument("paths", nargs="*")

    args = parser.parse_args()

    rows = []

    paths = args.paths

    for path in paths:
        for path, dirs, files in os.walk(path):
            for file in files:
                if file.endswith(".su"):
                    full_path = os.path.join(path, file)
                    with open(full_path) as f:
                        for line in f.readlines():
                            fields = line.split("\t")
                            fn = fields[0]
                            stack = fields[1]
                            rows.append(Row(fn, int(stack)))

    for row in sorted(rows, key=lambda r: (r.size, r.fn)):
        print("%s %s" % (row.fn, row.size))


if __name__ == "__main__":
    main()
