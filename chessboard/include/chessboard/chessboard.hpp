#pragma once

#include "chessboard/cornersort.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

#include <initializer_list>
#include <unordered_map>

class Chessboard final {
    using pii = std::pair<int, int>;

public:
    Chessboard(cv::Mat mat, const std::initializer_list<pii>& patternsizes, \
        cornersort::CameraPosition cameraposition = cornersort::CameraPosition::none);
    void init(cv::Mat mat, const std::initializer_list<pii>& patternsizes, \
        cornersort::CameraPosition cameraposition = cornersort::CameraPosition::none);
    std::vector<std::vector<cv::Point>> getChessboards();

private:
    // static const inline
    const cv::Mat crosskernel_;
    const cv::Mat rectkernel_;
    std::unordered_map<cornersort::ChessboardType, std::array<int, 5>> chessboardtypequadinfo_;

    cv::Mat mat_;
    std::vector<pii> patternsizes_;
    std::vector<pii> blockcorners_;
    cornersort::CameraPosition cameraposition_;

    std::vector<std::vector<cv::Point>> getChessboards(int blocksize, int constantc);

    void markRectArea(cv::Mat& image, const std::vector<cv::Point>& points);

    std::vector<std::vector<cv::Point>> getQuads(const cv::Mat& threshmat) const;
    std::vector<std::vector<cv::Point>> connectQuads(std::vector<std::vector<cv::Point>>& quads, int erosion = 0);

    bool isValid() const;
    double quadMinEdgeLength(const std::vector<cv::Point>& quad) const;
    bool checkLinePosition(const std::vector<cv::Point>& quada, const std::vector<cv::Point>& quadb, const cv::Point& pa, const cv::Point& pb) const;
};
