#!/usr/bin/python3

import os
import subprocess

source_files = [
    "commutators_real_space_1d.cpp",
    "commutators_real_space_2d.cpp",
    "example.cpp"
]

script_path = os.path.dirname(os.path.realpath(__file__))
project_path = os.path.join(script_path, os.pardir)
include_path = os.path.join(project_path, "include")
external_path = os.path.join(project_path, "external")

hpp_files = []
for root, _, files in os.walk(include_path):
    for f in files:
        if f.endswith(".hpp"):
            hpp_files.append(os.path.join(root, f))

cxx_flags = [
    "-std=c++11",
    "-isystem", os.path.join(external_path, "eigen"),
    "-isystem", os.path.join(external_path, "catch", "include"),
    "-I", include_path
]

iwyu_flags = []
for f in hpp_files:
    iwyu_flags.append("-Xiwyu")
    iwyu_flags.append("--check_also=" + f)

source_files = [os.path.join(project_path, f) for f in source_files]
basic_cmd = ["include-what-you-use"] + iwyu_flags + cxx_flags

for f in source_files:
    print("NEXT: " + f)
    cmd = basic_cmd + [f]
    proc = subprocess.run(cmd)
    print(proc.stderr)
    print("\n\n")
