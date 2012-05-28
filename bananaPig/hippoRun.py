#!/usr/bin/env python
import os
import sys

cmds = [
    "./emma ",
    "./emma -p ",
    "./emma -h ",
    "./emma -a ",
    ]

suffixs = [
    ".null",
    ".prefetch",
    ".history",
    ".all"
    ]

delta = 0
stop = 0 
while True:
    for i in range(len(cmds)):
        cmd = cmds[i] + "-d" + str(delta) + " | tee result/" + str(delta) + suffixs[i]
        print cmd
        ret = os.system(cmd)
        if ret != 0:
            print "ret:",ret
            stop += 1
            print "stop:",stop
            if stop >= 5:
                exit(1)
        else:
            stop = 0
    delta += 1
