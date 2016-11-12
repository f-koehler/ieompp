#!/usr/bin/env python3
import re
from os import walk
from os.path import join, relpath, splitext

regex_include_guard = re.compile(r"^#ifndef (IEOMPP_\w+)\n#define (IEOMPP_\w+)$", re.MULTILINE)

if __name__ == "__main__":
    retval = 0
    for root, _, files in walk("include/ieompp"):
        for file in files:
            path = join(root, file)
            if splitext(path)[1] != ".hpp":
                continue
            with open(path) as f:
                code = f.read()
            m = regex_include_guard.search(code)
            if not m:
                print("Missing include guard in \"" + path + "\"")
                retval = 1
                continue
            str1 = m.group(1)
            str2 = m.group(2)
            if str1 != str2:
                print("Invalid include guard in \"" + path + "\":")
                print("\tConstants are unequal (" + str1 + " != " + str2 + ")")
                retval = 1
                continue

            expected_constant = relpath(path, "include")
            expected_constant = expected_constant.upper()
            expected_constant = expected_constant.replace(".", "_")
            expected_constant = expected_constant.replace("/", "_")
            expected_constant += "_"

            if str1 != expected_constant:
                print("Invalid include guard in \"" + path + "\":")
                print("\tInvalid constant used (" + str1 + " != " + expected_constant + ")")
                retval = 1
                continue
    exit(retval)
