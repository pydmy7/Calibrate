#include "chessboard/cornersort.hpp"


#include "chessboard/geometry.hpp"

#include <numeric>
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


namespace {

std::pair<int, int> getChessboardSizeByChessboardType(cornersort::ChessboardType chessboardtype) {
    if (chessboardtype == cornersort::ChessboardType::twomultwo) {
        return {2, 2};
    } else if (chessboardtype == cornersort::ChessboardType::threemulthree) {
        return {3, 3};
    } else if (chessboardtype == cornersort::ChessboardType::threemulfour) {
        return {3, 4};
    } else {
        return {0, 0};
    }
}

void sortCorners(std::vector<cv::Point>& corners, cornersort::CameraPosition cameraposition) {
    cornersort::ChessboardType chessboardtype = cornersort::ChessboardCornerCntToChessboardType(corners.size());

    if (chessboardtype == cornersort::ChessboardType::threemulthree) {
        sortCornersForThreeMulThree(corners);
    } else if (chessboardtype == cornersort::ChessboardType::threemulfour) {
        sortCornersForThreeMulFour(corners);
        return;
    } else if (chessboardtype == cornersort::ChessboardType::twomultwo) {
        sortCornersForTwoMulTwo(corners);
    } else {
        return;
    }

    if (cameraposition == cornersort::CameraPosition::left \
        || cameraposition == cornersort::CameraPosition::frontleft \
        || cameraposition == cornersort::CameraPosition::rearleft
    ) {
        rotateByMatrix(corners, 3);
    } else if (
        cameraposition == cornersort::CameraPosition::right \
        || cameraposition == cornersort::CameraPosition::frontright \
        || cameraposition == cornersort::CameraPosition::rearright
    ) {
        rotateByMatrix(corners, 1);
    } else if (cameraposition == cornersort::CameraPosition::rear) {
        std::reverse(corners.begin(), corners.end());
    } else {
        // assert(cameraposition == cornersort::CameraPosition::front || cameraposition == cornersort::CameraPosition::none);
    }
}

void sortCornersForThreeMulThree(std::vector<cv::Point>& corners) {
    auto getFarthesTowPoint = [](const std::vector<cv::Point>& points, geometry::Line<int> line) -> std::pair<cv::Point, cv::Point> {
        std::vector<std::pair<cv::Point, double>> res;
        for (const cv::Point& point : points) {
            double curdist = geometry::pointToLineDistance(point, line);
            if (int(res.size()) == 0) {
                res.emplace_back(point, curdist);
            } else if (int(res.size()) == 1) {
                res.emplace_back(point, curdist);
                if (res[0].second < res[1].second) {
                    std::swap(res[0], res[1]);
                }
            } else {
                if (curdist > res[0].second) {
                    res[1] = res[0];
                    res[0] = {point, curdist};
                } else if (curdist > res[1].second) {
                    res[1] = {point, curdist};
                }
            }
        }
        return {res[0].first, res[1].first};
    };

    std::vector<cv::Point> newcorners(corners.size());

    newcorners[4] = getClosestPoint(std::accumulate(corners.begin(), corners.end(), cv::Point{0, 0}) / 9, corners);

    auto [startpoint, endpoint] = getFarthestPointPair(corners);
    if (startpoint.y > endpoint.y) {
        std::swap(startpoint, endpoint);
    }

    std::array<std::pair<int, int>, 2> transformidx = {
        std::pair<int, int>{0, 8},
        std::pair<int, int>{2, 6}
    };
    if (geometry::angle(endpoint - startpoint, cv::Point{1, 0}) / 3.1415926535L > 90 / 180.0L) {
        std::swap(transformidx[0], transformidx[1]);
    }
    newcorners[transformidx[0].first] = startpoint;
    newcorners[transformidx[0].second] = endpoint;
    std::tie(startpoint, endpoint) = getFarthesTowPoint(corners, geometry::Line{newcorners[transformidx[0].first], newcorners[transformidx[0].second]});
    if (startpoint.y > endpoint.y) {
        std::swap(startpoint, endpoint);
    }
    newcorners[transformidx[1].first] = startpoint;
    newcorners[transformidx[1].second] = endpoint;

    for (int i : {1, 3, 5, 7}) {
        if (i == 1 || i == 7) {
            newcorners[i] = getClosestPoint((newcorners[i - 1] + newcorners[i + 1]) / 2, corners);
        } else {
            newcorners[i] = getClosestPoint((newcorners[i - 3] + newcorners[i + 3]) / 2, corners);
        }
    }

    corners = std::move(newcorners);
}

void sortCornersForThreeMulFour(std::vector<cv::Point>& corners) {
    std::sort(corners.begin(), corners.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.y < rhs.y || lhs.y == rhs.y && lhs.x < rhs.x;
    });
    for (int i = 0; i < int(corners.size()); i += 3) {
        std::sort(corners.begin() + i, corners.begin() + i + 3, [](const auto& lhs, const auto& rhs) {
            return lhs.x < rhs.x;
        });
    }
}

void sortCornersForTwoMulTwo(std::vector<cv::Point>& corners) {
    std::sort(corners.begin(), corners.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.y < rhs.y || lhs.y == rhs.y && lhs.x < rhs.x;
    });
    for (int i = 0; i < int(corners.size()); i += 2) {
        std::sort(corners.begin() + i, corners.begin() + i + 2, [](const auto& lhs, const auto& rhs) {
            return lhs.x < rhs.x;
        });
    }
}

std::pair<cv::Point, cv::Point> getFarthestPointPair(const std::vector<cv::Point>& points) {
    double maxdist = 0;
    std::pair<cv::Point, cv::Point> res;
    for (int n = points.size(), i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double curdist = geometry::getDistance(points[i], points[j]);
            if (curdist > maxdist) {
                maxdist = curdist;
                res = {points[i], points[j]};
            }
        }
    }
    return res;
}

cv::Point getClosestPoint(const cv::Point& p, const std::vector<cv::Point>& points) {
    double closestdist = geometry::getDistance(p, points[0]);
    cv::Point closestpoint = points[0];
    for (std::size_t i = 1; i < points.size(); ++i) {
        double curdist = geometry::getDistance(p, points[i]);
        if (curdist < closestdist) {
            closestdist = curdist;
            closestpoint = points[i];
        }
    }
    return closestpoint;
}

void rotateByMatrix(std::vector<cv::Point>& corners, int cnt) {
    cornersort::ChessboardType chessboardtype = cornersort::ChessboardCornerCntToChessboardType(corners.size());
    const auto [row, col] = getChessboardSizeByChessboardType(chessboardtype);
    std::vector<std::vector<cv::Point>> matrix(row, std::vector<cv::Point>(col));

    for (std::size_t i = 0; i < matrix.size(); ++i) {
        for (std::size_t j = 0; j < matrix[i].size(); ++j) {
            matrix[i][j] = corners[i * matrix[i].size() + j];
        }
    }

    for (int i = 0; i < cnt; ++i) {
        rotateMatrix(matrix);
    }

    for (std::size_t i = 0; i < matrix.size(); ++i) {
        for (std::size_t j = 0; j < matrix[i].size(); ++j) {
            corners[i * matrix[i].size() + j] = matrix[i][j];
        }
    }
}

void rotateMatrix(std::vector<std::vector<cv::Point>>& matrix) {
    const int n = matrix.size();
    for (int i = 0; i < n / 2; ++i) {
        for (int j = 0; j < (n + 1) / 2; ++j) {
            std::tie(matrix[i][j], matrix[n - j - 1][i], matrix[n - i - 1][n - j - 1], matrix[j][n - i - 1])
                = std::make_tuple(matrix[n - j - 1][i], matrix[n - i - 1][n - j - 1], matrix[j][n - i - 1], matrix[i][j]);
        }
    }
}

}  // namespace
