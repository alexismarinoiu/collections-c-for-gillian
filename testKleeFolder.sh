#!/bin/bash

folder=$1
rm -r $folder/klee-out-*

count=0
time {
for filename in $folder/*.c; do
    [ -f "$filename" ] || break
    echo ""
    echo "GOING TO ANALYSE : $filename"
    echo ""
    echo "" >> klee_output.log
    echo "GOING TO ANALYSE : $filename" >> klee_output.log
    time (clang -I $KLEE_INCLUDE -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone $filename > /dev/null 2>&1)
    klee -libc=klee $(basename $filename .c).bc >> klee_output.log 2>&1
    cat klee-out-$count/info | grep walltimer
    count=$((count+1))
done
}

rm -rf *.bc *.log klee-last klee-out-*
