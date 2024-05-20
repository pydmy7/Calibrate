#### features

- [X] find chessboard corner
- [X] corner sort
- [X] async concurrent
- [X] platform: Linux / Windows


#### requirements
`linux`
```sh
sudo apt install -y clang ninja-build cmake libopencv-dev
```
`windows`
```powershell
scoop install llvm ninja cmake
```


#### run test demo

```sh
./make.sh  # Linux
./make.ps1  # Windows
```


#### how to use

```bash
git submodule add https://github.com/pydmy7/Calibrate.git 3rdparty/Calibrate
git submodule update --init --recursive
```

```cmake
add_subdirectory(Calibrate)
target_link_libraries(you-target PRIVATE Calibrate::Chessboard)
```
