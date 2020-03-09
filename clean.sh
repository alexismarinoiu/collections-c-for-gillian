#/bin/bash

find . -name \*.csm -type f -delete
find . -name \*.i -type f -delete
find . -name \*.bc -type f -delete
rm -rf klee-out-*
rm -f klee_output.log
rm -rf klee-last