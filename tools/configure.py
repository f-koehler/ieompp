#!/usr/bin/env python3
from subprocess import check_output
from os.path import exists

path = "include/ieompp/version.hpp"

try:
    version = check_output(["git", "describe", "--dirty"]).decode().strip()
except:
    version = "0.0.0-unknown"

config = (
    "#ifndef IEOMPP_VERSION_HPP_\n"
    "#define IEOMPP_VERSION_HPP_\n"
    "\n"
    "#include <string>\n"
    "\n"
    "namespace ieompp {\n"
    "    constexpr auto version = \"%s\";\n"
    "} // ieompp\n"
    "\n"
    "#endif"
) % (version)

if not exists(path):
    with open(path, "w") as f:
        f.write(config)
    exit()

with open(path) as f:
    current_config = f.read()

if config != current_config:
    with open(path, "w") as f:
        f.write(config)
