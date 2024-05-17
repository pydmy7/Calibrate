#pragma once


#include <opencv2/core/types.hpp>

#include <vector>
#include <utility>


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


namespace {

std::pair<int, int> getChessboardSizeByChessboardType(cornersort::ChessboardType chessboardtype);

void sortCorners(std::vector<cv::Point>& corners, cornersort::CameraPosition cameraposition);
void sortCornersForThreeMulThree(std::vector<cv::Point>& corners);
void sortCornersForThreeMulFour(std::vector<cv::Point>& corners);
void sortCornersForTwoMulTwo(std::vector<cv::Point>& corners);

std::pair<cv::Point, cv::Point> getFarthestPointPair(const std::vector<cv::Point>& points);
cv::Point getClosestPoint(const cv::Point& p, const std::vector<cv::Point>& points);

void rotateByMatrix(std::vector<cv::Point>& corners, int cnt);
void rotateMatrix(std::vector<std::vector<cv::Point>>& matrix);

}  // namespace
