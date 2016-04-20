#!/bin/bash

gcc malloctest.c -lpthread
./a.out > data_wear_glibc

gcc malloctest.c -lpthread -ltcmalloc_minimal
./a.out > data_wear_tcmalloc

g++ stat_dist.cpp
./a.out data_wear_glibc
echo "----"
./a.out data_wear_tcmalloc
