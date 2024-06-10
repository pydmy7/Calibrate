#!/bin/bash

clear
echo -e "clear"

echo -e "\nrm build/CMakeCache.txt"
rm build/CMakeCache.txt

echo -e "\nsource: cmake -S . -B build -G Ninja"
cmake -S . -B build -G Ninja

echo -e "\nbuild: cmake --build build -j"
cmake --build build -j

echo -e "\nrun: ./build/unittest/unittest"
./build/unittest/unittest
