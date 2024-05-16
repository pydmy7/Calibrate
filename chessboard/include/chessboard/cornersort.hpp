#pragma once

#include <opencv2/core/types.hpp>

#include <vector>
#include <unordered_map>
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

// -----------------------------------------------------------------------------------------------------------

static std::pair<int, int> getChessboardSizeByChessboardType(ChessboardType chessboardtype);

static void sortCorners(std::vector<cv::Point>& corners, CameraPosition cameraposition);
static void sortCornersForThreeMulThree(std::vector<cv::Point>& corners);
static void sortCornersForThreeMulFour(std::vector<cv::Point>& corners);
static void sortCornersForTwoMulTwo(std::vector<cv::Point>& corners);

static std::pair<cv::Point, cv::Point> getFarthestPointPair(const std::vector<cv::Point>& points);
static cv::Point getClosestPoint(const cv::Point& p, const std::vector<cv::Point>& points);

static void rotateByMatrix(std::vector<cv::Point>& corners, int cnt);
static void rotateMatrix(std::vector<std::vector<cv::Point>>& matrix);

}
