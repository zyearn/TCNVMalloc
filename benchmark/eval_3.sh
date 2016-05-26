#!/bin/bash

gcc -o wa.out eval_3.c ../src/tcnvmalloc.c ../src/pq.c -I../src/ -DWA_MALLOC -lpthread
gcc -o nv.out eval_3.c nvmalloc/nvmalloc.c -DNV_MALLOC -lpthread
g++ -o stat_dist stat_dist.cpp

i="0"
BASE="10000"
SLOP="10000"
THNUM=1

while [ $i -lt 15 ]
do
    TIMES=$(($BASE+$i*$SLOP))
    echo -n $TIMES" "
    ./wa.out $TIMES $THNUM | ./stat_dist
    echo -n " "
    ./nv.out $TIMES $THNUM | ./stat_dist
    echo ""

    i=$(($i+1))
done
