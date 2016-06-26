#!/usr/bin/python3
import os


def loc_task(directory):
    s = 0
    for root, _, files in os.walk(directory):
        for f in files:
            if os.path.splitext(f)[1] not in [".cpp", ".hpp"]:
                continue
            with open(os.path.join(root, f)) as strm:
                s += sum(1 for _ in strm)
    return s

directories = ["include", "tests", "src"]

s = 0
for d in directories:
    c = loc_task(d)
    s += c
    print(d + ":\t" + str(c))

print("\ntotal:\t" + str(s))
