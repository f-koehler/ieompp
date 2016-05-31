#!/usr/bin/python3
import sys
import os
import subprocess
import time

script_path = os.path.dirname(os.path.realpath(__file__))
with open(os.path.join(script_path, "render.tex")) as f:
    template = f.read()
lines = [l for l in sys.stdin]

tex_code = [r"&" + l.strip() + r"\\" for l in lines]
tex_code = "\n".join(tex_code)
tex_code = template.replace("{{ subst }}", tex_code)

if not os.path.exists("/tmp/ieompp-render"):
    os.mkdir("/tmp/ieompp-render")
name = time.strftime("%H-%M-%S")
print(tex_code)
p = subprocess.Popen(["xelatex", "--jobname=" + name, "--output-directory=/tmp/ieompp-render"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
stdout, stderr = p.communicate(tex_code.encode())
