#!/bin/bash

clear
echo -e "clear"

echo -e "\nrm -rf build"
rm -rf build

echo -e "\nsource: cmake -S . -B build -G Ninja -D MYSELF=ON"
cmake -S . -B build -G Ninja -D MYSELF=ON

echo -e "\nbuild: cmake --build build -j"
cmake --build build -j

echo -e "\nrun: ./build/unittest/unittest"
./build/unittest/unittest
