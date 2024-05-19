#!/bin/bash

clear
echo -e "clear"

echo -e "\nrm -rf build"
rm -rf build

echo -e "\nsource: cmake -S . -B build -G Ninja"
cmake -S . -B build -G Ninja

echo -e "\nbuild: cmake --build build -j"
cmake --build build -j

echo -e "\nrun: ./build/test/test"
./build/test/test
