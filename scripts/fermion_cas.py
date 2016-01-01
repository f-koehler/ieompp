#!/bin/env python3
import hashlib
import sys
import subprocess
import os.path as path

executable = "./fermion_cas"
tex = r"""
\documentclass{scrartcl}
\usepackage{unicode-math}
\usepackage{amsmath}
\begin{document}
  $ %input% = %result% $
\end{document}
"""

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: fermion_cas.py [commutator]", sys.stderr)
        exit(1)

    output = subprocess.check_output([executable, sys.argv[1]]).decode()
    digest = hashlib.sha512(output.encode()).hexdigest()
    text = tex.replace("%input%", sys.argv[1]).replace("%result%", output)
    with open(path.join("/tmp", digest+".tex"), "w") as f:
        f.write(text)

    subprocess.check_output(["lualatex", "--output-directory=/tmp", "--interaction=errorstopmode", "--halt-on-error", path.join("/tmp", digest+".tex")])
