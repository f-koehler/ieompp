#!/usr/bin/env python3
from os import walk
from os.path import join, splitext
from re import compile

regex_bad_include = compile(r"^\s*#\s*include\s*<(ieompp/.*\.hpp)>\s*$")

if __name__ == "__main__":
    for root, _, files in walk("include/ieompp"):
        for file in files:
            path = join(root, file)
            if splitext(path)[1] != ".hpp":
                continue
            with open(path) as f:
                contents = f.readlines()
            print(path + ":")
            for idx, line in enumerate(contents):
                m = regex_bad_include.match(line)
                if not m:
                    continue
                new_line = "#include \"" + m.group(1) + "\""
                print("{}:\t{} -> {}".format(idx, line.strip(), new_line))
                contents[idx] = new_line + "\n"
            with open(path, "w") as f:
                f.writelines(contents)
