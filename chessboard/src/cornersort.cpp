#include "chessboard/cornersort.hpp"

#include "chessboard/cornersorthelper.hpp"

#include <algorithm>

namespace cornersort {

void areaCornerSort(std::vector<std::vector<cv::Point>>& areas, CameraPosition cameraposition) {
    for (auto& area : areas) {
        sortCorners(area, cameraposition);
    }

    if (cameraposition == CameraPosition::none \
        || cameraposition == CameraPosition::front
    ) {
        std::sort(areas.begin(), areas.end(), [](const auto& lhs, const auto& rhs) {
            return lhs[0].x + lhs[0].y < rhs[0].x + rhs[0].y;
        });
    } else if (cameraposition == CameraPosition::left \
        || cameraposition == CameraPosition::frontleft \
        || cameraposition == CameraPosition::rearleft
    ) {
        std::sort(areas.begin(), areas.end(), [](const auto& lhs, const auto& rhs) {
            return lhs[0].x + lhs[0].y > rhs[0].x + rhs[0].y;
        });
    } else if (cameraposition == CameraPosition::right \
        || cameraposition == CameraPosition::frontright \
        || cameraposition == CameraPosition::rearright
    ) {
        std::sort(areas.begin(), areas.end(), [](const auto& lhs, const auto& rhs) {
            return lhs[0].x + lhs[0].y < rhs[0].x + rhs[0].y;
        });
    } else {
        // assert(cameraposition == CameraPosition::rear);
        std::sort(areas.begin(), areas.end(), [](const auto& lhs, const auto& rhs) {
            return lhs[0].x + lhs[0].y > rhs[0].x + rhs[0].y;
        });
    }
}

ChessboardType ChessboardCornerCntToChessboardType(int cnt) {
    if (cnt == 4) {
        return ChessboardType::twomultwo;
    } else if (cnt == 9) {
        return ChessboardType::threemulthree;
    } else if (cnt == 12) {
        return ChessboardType::threemulfour;
    } else {
        return ChessboardType::none;
    }
}

}  // namespace cornersort
