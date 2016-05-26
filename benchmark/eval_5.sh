#!/bin/bash

gcc -o wa.out       eval_5.c ../src/tcnvmalloc.c ../src/pq.c -I../src/ -DWA_MALLOC -lpthread
gcc -o nv.out       eval_5.c nvmalloc/nvmalloc.c -DNV_MALLOC -lpthread
gcc -o glibc.out    eval_5.c -lpthread
g++ -o stat_dist stat_dist.cpp

i="1"
TIMES="100000"

while [ $i -lt 11 ]
do
    echo -n $i" "
    ./wa.out $TIMES $i
    echo -n " "
    ./nv.out $TIMES $i
    echo -n " "
    ./glibc.out $TIMES $i
    echo ""

    i=$(($i+1))
done
