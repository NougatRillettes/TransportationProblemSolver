import time
import subprocess
import tempfile
import resource
import sys
for i in [i*50 for i in range(75)]:
    print(i,end="\t")
    f = tempfile.NamedTemporaryFile()
    subprocess.call(["python","generator.py",str(i)],stdout=f)
    s = subprocess.run(["time","-f%U","./a.out",f.name],stderr=subprocess.PIPE,stdout=subprocess.DEVNULL)
    print(s.stderr[:-1].decode())
