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

sqls = [
    "places.sqlite",
    ]

nows = [
    333333333,
    ]
delta = 0
stop = 0 

for s in range(len(sqls)):
    sql = "external/"+sqls[s]
    now = str(nows[s])
    resultDir = sql+".result"
    os.system("mkdir " + resultDir)
    for i in range(len(cmds)):
        cmd = cmds[i] + "-d" + str(delta) + ' -s' + sql + " -n" + now + " | tee " + resultDir + "/" + str(delta) + suffixs[i] 
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
