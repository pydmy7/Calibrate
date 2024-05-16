#### 最低版本要求

-std=c++17, cmake 3.15, opencv 4.x.

#### Task

- [ ] 多线程并行计算汇总所有结果

#### features

- [X] `3 * 3` / `3 * 4` 棋盘找角点
- [X] `2 * 2` / `3 * 3` / `3 * 4` 角点排序
- [X] 跨平台 Windows / Linux

#### run test demo

```sh
./make.sh  # Linux
./make.ps1  # Windows
```

#### how to use

```bash
sudo apt install libopencv-dev  # Linux需要安装opencv，Windows不需要配置依赖
git submodule add https://gt.xtad.cn:7571/common/calibrate.git 3rdparty/
git submodule update
```

```cmake
add_subdirectory(calibrate)
target_link_libraries(you-target PRIVATE Calibrate::Chessboard)
```

## 找角点算法

参考

1. Automatic Detection of Checkerboards on Blurred and Distorted Images论文
2. opencv: cv::findChessboardCorners源码
3. 权华的实现找角点算法

opencv为什么找不到角点？

opencv对于四边形的筛选过于严格：有一些棋盘是能够找到的，由于相机的畸变与四边形的扭曲，导致这些四边形不是那么的标准，opencv就会认为这类四边形是杂质。

权华的算法为什么找不到角点？

## 排序算法

下面只讨论3*3的棋盘。

1.确定中点

根据九个向量的算术平均数可以得到虚拟中点，棋盘中的九个点距离虚拟中点最近的点就是真实的中点。

2.确定任意一组对角

去掉上边已经确定的中点，我们还剩下八个点，任意从八个点中选两个点，距离最远的两个点一定是一组对角。

3.确定另外一组对角

现在我们还剩下六个点，找到距离 `2.确定任意一组对角`这组对角线距离最远的两个点，就是另外一组对角点。

4.确定剩下的四条边上的中点

每个中点都是该边另外两个端点的虚拟中点的临近点。
