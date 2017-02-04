# -*- coding: utf-8 -*-
import sys

for line in sys.stdin:
    line = line.strip()
    elems = line.split(' ')
    for elem in elems:
        (word, tag) = elem.split('_')
        print("{} {}".format(word, tag))
    print("\n")
