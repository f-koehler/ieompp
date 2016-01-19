#!/bin/env python
from subprocess import check_output
from os.path import exists

try:
    version = check_output(["git", "describe", "--dirty"]).decode().strip()
except:
    version = "0.0.0-unknown"

config = (
    "#ifndef HUBBARD_VERSION_HPP_\n"
    "#define HUBBARD_VERSION_HPP_\n"
    "\n"
    "#include <string>\n"
    "\n"
    "namespace hubbard {\n"
    "    constexpr auto version = \"%s\";\n"
    "}\n"
    "\n"
    "#endif"
) % (version)

if not exists("include/hubbard/config.hpp"):
    with open("include/hubbard/config.hpp", "w") as f:
        f.write(config)
    exit()

with open("include/hubbard/config.hpp") as f:
    current_config = f.read()

if config != current_config:
    with open("include/hubbard/config.hpp", "w") as f:
        f.write(config)
