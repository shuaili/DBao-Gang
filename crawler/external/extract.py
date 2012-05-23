#!/usr/bin/env python
#coding:utf-8 
from BeautifulSoup import BeautifulSoup
import sys, os
reload(sys) 
sys.setdefaultencoding('utf-8') 

input = sys.argv[1]
if not input.endswith('.html'):
    print "[ERROR]input file name invalid", sys.argv[1]
    exit(-1)

input_file = open(input)

doc = input_file.read()

input_file.close()

soup = BeautifulSoup(doc)

scripts = soup.findAll('script')

output = sys.argv[1][:-5] + ".js"
outputFd = open(output, "w")
try:
    for i in range(len(scripts)):
        for j in range(len(scripts[i].contents)):
            outputFd.write(scripts[i].contents[j] + "\n")
except Exception as e:
    print e
    print "[EXCEPT]", scripts[i].contents[j]


outputFd.close()
