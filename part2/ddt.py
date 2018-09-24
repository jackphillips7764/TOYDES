#!/usr/bin/env python2
import sys
def getcol(x):
    x = x >> 1
    x &= 3
    return x

def getrow(x):
    y = x & 1
    y |=( (x>>2) & 2)
    return y

sbox = [[1,0,3,2],[3,2,1,0],[0,2,1,3],[3,1,3,2]]

ddt = []
for i in xrange(16):
    ddt.append([0,0,0,0])
print ddt
print len(ddt)
for x in xrange(16):
    for xstar in xrange(16):
        xprime = x ^ xstar
        y = sbox[getcol(x)][getrow(x)]
        ystar = sbox[getcol(xstar)][getrow(xstar)]
        yprime = y ^ ystar
        ddt[xprime][yprime] += 1
        if xprime == 4 and yprime == 1:
            print "FOUND:: " + str(x) + ", " + str(xstar) 

print ddt
for i in ddt:
    for x in i:
        sys.stdout.write(str(x) + " ")
    print ""
