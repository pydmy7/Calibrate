clear
echo "clear"

echo "`nRemove-Item -Path .\build\CMakeCache.txt"
Remove-Item -Path .\build\CMakeCache.txt

echo "`nsource: cmake -S . -B build -G Ninja"
cmake -S . -B build -G Ninja

echo "`nbuild: cmake --build build -j"
cmake --build build -j

echo "`nrun: .\build\unittest\unittest.exe"
.\build\unittest\unittest.exe
