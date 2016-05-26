#!/bin/bash

gcc -o wa.out eval_2.c ../src/tcnvmalloc.c ../src/pq.c -I../src/ -DWA_MALLOC -lpthread
gcc -o nv.out eval_2.c nvmalloc/nvmalloc.c -DNV_MALLOC -lpthread

i="0"
BASE="10000"
SLOP="10000"

while [ $i -lt 15 ]
do
    TIMES=$(($BASE+$i*$SLOP))
    echo -n $TIMES" "
    echo -n `(./memusg.sh ./wa.out $TIMES 8)`
    echo -n " "
    echo -n `(./memusg.sh ./nv.out $TIMES 8)`
    echo ""

    i=$(($i+1))
done
