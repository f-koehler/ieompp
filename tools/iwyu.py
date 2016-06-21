#!/usr/bin/python3

import os
import subprocess
import jinja2

source_files = [
    "commutators.cpp",
    "commutators_real_space_1d.cpp",
    "commutators_real_space_2d.cpp",
    "tests/algebra/test_anticommutator.cpp",
    "tests/algebra/test_operator.cpp",
]

script_path = os.path.dirname(os.path.realpath(__file__))
project_path = os.path.join(script_path, os.pardir)
include_path = os.path.join(project_path, "include")
external_path = os.path.join(project_path, "external")
template_path = os.path.join(project_path, "tools", "templates")

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

iwyu_flags = ["-Xiwyu", "--verbose=0"]
for f in hpp_files:
    iwyu_flags.append("-Xiwyu")
    iwyu_flags.append("--check_also=" + f)

source_files = [os.path.join(project_path, f) for f in source_files]
basic_cmd = ["include-what-you-use"] + iwyu_flags + cxx_flags

iwyu_output = []
for f in source_files:
    rel = os.path.relpath(f, project_path)
    cmd = basic_cmd + [f]
    print(rel)
    proc = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    stdout, stderr = proc.communicate()
    stderr = stderr.decode()
    stderr = stderr.replace("<", "&lt;")
    stderr = stderr.replace(">", "&gt;")
    iwyu_output.append((rel, stderr))

env = jinja2.Environment(loader=jinja2.FileSystemLoader(template_path))
template = env.get_template("iwyu.html")
html = template.render(results=iwyu_output)

with open(os.path.join(project_path, "iwyu.html"), "w") as f:
    f.write(html)
