#### minimul version required
-std=c++17, cmake 3.15, opencv 4.x.
```sh
sudo apt install g++ cmake libopencv-dev
```

#### features

- [X] find chessboard corner
- [X] corner sort
- [X] async concurrent
- [X] platform: Linux / Windows

#### run test demo

```sh
./make.sh  # Linux
./make.ps1  # Windows
```

#### how to use

```bash
git submodule add https://github.com/pydmy7/Calibrate.git
```

```cmake
add_subdirectory(Calibrate)
target_link_libraries(you-target PRIVATE Calibrate::Chessboard)
```
