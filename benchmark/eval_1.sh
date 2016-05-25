#/bin/bash

gcc -o wa.out wear_1.c ../src/tcnvmalloc.c ../src/pq.c -I../src/ -DWA_MALLOC -lpthread
gcc -o nv.out wear_1.c nvmalloc/nvmalloc.c -DNV_MALLOC -lpthread
g++ -o stat_dist stat_dist.cpp

i="0"
BASE="5000"
SLOP="5000"

while [ $i -lt 15 ]
do
    TIMES=$(($BASE+$i*$SLOP))
    echo -n $TIMES" "
    ./wa.out $TIMES 8 | ./stat_dist
    echo -n " "
    ./nv.out $TIMES 8 | ./stat_dist
    echo ""

    i=$(($i+1))
done
