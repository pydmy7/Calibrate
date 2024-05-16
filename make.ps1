clear
echo "clear"

echo "`nRemove-Item -Path .\cmake-build\ -Recurse -Force"
Remove-Item -Path .\cmake-build\ -Recurse -Force

echo "`nsource: cmake -S . -B cmake-build"
cmake -S . -B cmake-build

echo "`nbuild: cmake --build cmake-build -j"
cmake --build cmake-build -j

echo "`nrun: .\cmake-build\test\Debug\test.exe"
.\cmake-build\test\Debug\test.exe
