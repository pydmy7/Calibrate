#### Overview
This project implements the methods and experiments described in the following research article:  
[Automatic detection of checkerboards on blurred and distorted images](https://ieeexplore.ieee.org/document/4650703), IEEE, 2008.


#### Features

- [X] find chessboard corner
- [X] corner sort
- [X] async concurrent
- [X] platform: Linux / Windows

#### Requirements

`Linux`

```sh
sudo apt install clang ninja-build cmake libopencv-dev
```

`Windows`

```powershell
scoop install llvm ninja cmake
```

#### Run test demo

```sh
./make.sh  # Linux
./make.ps1  # Windows
```

#### How to use

```bash
git submodule add https://github.com/pydmy7/Calibrate.git 3rdparty/Calibrate
git submodule update --init --recursive
```

```cmake
add_subdirectory(Calibrate)
target_link_libraries(you-target PRIVATE Calibrate::Chessboard)
```
