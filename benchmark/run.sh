#!/bin/bash

g++ -o stat_dist stat_dist.cpp

gcc malloctest.c -lpthread
./a.out $1 | ./stat_dist

echo "----"

gcc malloctest.c ../src/tcnvmalloc.c -I../src/ -DNV_MALLOC -lpthread
./a.out $1 | ./stat_dist

#gcc malloctest.c -lpthread -ltcmalloc_minimal
#./a.out | ./stat_dist
