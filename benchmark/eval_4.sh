#!/bin/bash

gcc -o wa.out eval_4.c ../src/tcnvmalloc.c ../src/pq.c -I../src/ -DWA_MALLOC -lpthread
gcc -o nv.out eval_4.c nvmalloc/nvmalloc.c -DNV_MALLOC -lpthread

i="0"
BASE="10000"
SLOP="10000"
THNUM="1"

while [ $i -lt 15 ]
do
    TIMES=$(($BASE+$i*$SLOP))
    echo -n $TIMES" "
    echo -n `(./memusg.sh ./wa.out $TIMES $THNUM)`
    echo -n " "
    echo -n `(./memusg.sh ./nv.out $TIMES $THNUM)`
    echo ""

    i=$(($i+1))
done
