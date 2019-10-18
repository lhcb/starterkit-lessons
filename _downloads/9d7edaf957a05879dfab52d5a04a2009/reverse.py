#!/usr/bin/env python
import os
import sys

# Validate that we have been given one argument
# (The program name is always the first argument)
args = sys.argv
arg_count = len(args)
if arg_count != 2:
    print 'Usage: reverse.py <file>'
    sys.exit(1)

# Validate that the argument given is a valid file
input = args[1]
if not os.path.isfile(input):
    print '{0} is not a valid file'.format(input)
    print 'Usage: reverse.py <file>'
    sys.exit(1)

with open(input) as f:
    lines = f.readlines()
    reversed = [line[::-1] for line in lines[::-1]]

fname, extension = os.path.splitext(input)
output = fname + '-reversed' + extension
with open(output, 'w') as f:
    f.writelines(reversed)
