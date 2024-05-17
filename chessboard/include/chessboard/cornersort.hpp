#pragma once

#include <opencv2/core/types.hpp>

#include <vector>

namespace cornersort {

enum class CameraPosition {
    none,
    front,
    rear,
    left,
    right,
    frontleft,
    frontright,
    rearleft,
    rearright
};

enum class ChessboardType {
    none,
    twomultwo,
    threemulthree,
    threemulfour
};

void areaCornerSort(std::vector<std::vector<cv::Point>>& areas, CameraPosition cameraposition = CameraPosition::none);

ChessboardType ChessboardCornerCntToChessboardType(int cnt);

}  // namespace cornersort
