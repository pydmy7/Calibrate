#!/bin/bash

clear
echo -e "clear"

echo -e "\nrm -rf cmake-build"
rm -rf cmake-build

echo -e "\nsource: cmake -S . -B cmake-build"
cmake -S . -B cmake-build

echo -e "\nbuild: cmake --build cmake-build -j"
cmake --build cmake-build -j

echo -e "\nrun: ./cmake-build/test/test"
./cmake-build/test/test
