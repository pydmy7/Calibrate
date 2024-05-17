#include "chessboard/chessboard.hpp"

#include "chessboard/dsu.hpp"
#include "chessboard/geometry.hpp"
#include "chessboard/timer.hpp"

#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>

#include <cassert>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <array>
#include <vector>
#include <utility>
#include <unordered_map>
#include <map>
#include <future>

Chessboard::Chessboard(cv::Mat mat, const std::initializer_list<pii>& patternsizes, cornersort::CameraPosition cameraposition) : \
    crosskernel_(cv::getStructuringElement(cv::MORPH_CROSS, cv::Size{3, 3})), \
    rectkernel_(cv::getStructuringElement(cv::MORPH_RECT, cv::Size{3, 3})) \
{
    // 2 * 2 = 2 : 4
    chessboardtypequadinfo[cornersort::ChessboardType::twomultwo][2] = 4;
    // 3 * 3 = 1 : 2, 2 : 4, 4 : 2
    chessboardtypequadinfo[cornersort::ChessboardType::threemulthree][1] = 2;
    chessboardtypequadinfo[cornersort::ChessboardType::threemulthree][2] = 4;
    chessboardtypequadinfo[cornersort::ChessboardType::threemulthree][4] = 2;
    // 3 * 4 = 1 : 2, 2 : 5, 4 : 3
    chessboardtypequadinfo[cornersort::ChessboardType::threemulfour][1] = 2;
    chessboardtypequadinfo[cornersort::ChessboardType::threemulfour][2] = 5;
    chessboardtypequadinfo[cornersort::ChessboardType::threemulfour][4] = 3;

    init(mat, patternsizes, cameraposition);
}

void Chessboard::init(cv::Mat mat, const std::initializer_list<pii>& patternsizes, cornersort::CameraPosition cameraposition) {
    mat_ = mat;
    patternsizes_ = patternsizes;
    cameraposition_ = cameraposition;

    assert(isValid());

    if (mat_.channels() != 1) {
        cv::cvtColor(mat_, mat_, cv::COLOR_RGB2GRAY);
    }
    cv::equalizeHist(mat_, mat_);
    cv::GaussianBlur(mat_, mat_, cv::Size{3, 3}, 0);

    cv::rectangle(mat_, cv::Rect{cv::Point{0, 0}, cv::Point{mat_.cols - 1, mat_.rows - 1}}, cv::Scalar{255}, 3, cv::LINE_8);

    for (auto& [row, col] : patternsizes_) {
        if (row > col) {
            std::swap(row, col);
        }
        if (row == 2 && col == 2) {
            blockcorners_.emplace_back(4, 4);
        } else {
            blockcorners_.emplace_back((row - 1) * (col + 1), row * col);
        }
    }
}

std::vector<std::vector<cv::Point>> Chessboard::getChessboards() {
    Timer timer;

    std::vector<std::vector<cv::Point>> chessboards;

    std::vector<std::future<std::vector<std::vector<cv::Point>>>> futures;
    for (int blocksize = 51; blocksize <= 171; blocksize += 20) {
        for (int constantc = -3; constantc <= 6; constantc += 3) {
            futures.emplace_back(std::async([this, blocksize, constantc]() -> std::vector<std::vector<cv::Point>> {
                return this->getChessboards(blocksize, constantc);
            }));
        }
    }
    for (auto&& future : futures) {
        auto merge = [](std::vector<std::vector<cv::Point>>& ans, std::vector<std::vector<cv::Point>>&& tmp) {
            for (auto&& t : tmp) {
                ans.emplace_back(t);
            }
        };

        merge(chessboards, future.get());
    }

    std::sort(chessboards.begin(), chessboards.end(), [](const auto& lhs, const auto& rhs) -> bool {
        cv::Point lcentral = geometry::getCentralPoint(lhs);
        cv::Point rcentral = geometry::getCentralPoint(rhs);
        return lcentral.x < rcentral.x || (lcentral.x == rcentral.x && lcentral.y < rcentral.y);
    });

    std::vector<std::vector<cv::Point>> newchessboards;
    for (int n = chessboards.size(), i = 0; i < n; ++i) {
        // ---------------------------------------------------
        auto isSameChessboard = [&chessboards](int a, int b) -> bool {
            cv::Point pa = geometry::getCentralPoint(chessboards[a]);
            cv::Point pb = geometry::getCentralPoint(chessboards[b]);
            return std::abs(pa.x - pb.x) <= 5 && std::abs(pa.y - pb.y) <= 5;
        };

        auto getHighestFrequency = [&chessboards](int l, int r) -> std::vector<cv::Point> {
            auto vectorPointToVectorPair = [](auto&& points) -> std::vector<std::pair<int, int>> {
                std::vector<std::pair<int, int>> pairs;
                for (auto&& [x, y] : points) {
                    pairs.emplace_back(x, y);
                }
                return pairs;
            };

            auto vectorPairToVectorPoint = [](auto&& pairs) -> std::vector<cv::Point> {
                std::vector<cv::Point> points;
                for (auto&& [x, y] : pairs) {
                    points.emplace_back(x, y);
                }
                return points;
            };

            std::map<std::vector<std::pair<int, int>>, int> cnt;
            int maxcnt = 0;
            for (int i = l; i < r; ++i) {
                maxcnt = std::max(maxcnt, ++cnt[vectorPointToVectorPair(chessboards[i])]);
            }
            for (auto&& [chessboard, curcnt] : cnt) {
                if (curcnt == maxcnt) {
                    return vectorPairToVectorPoint(chessboard);
                }
            }
            assert(false);
            return {};
        };
        // ---------------------------------------------------

        int j = i + 1;
        while (j < n && isSameChessboard(i, j)) {
            ++j;
        }
        newchessboards.emplace_back(getHighestFrequency(i, j));
        i = j - 1;
    }
    chessboards = std::move(newchessboards);

    if (chessboards.size() < patternsizes_.size()) {
        for (int cnt = patternsizes_.size(), i = 0; i < cnt; ++i) {
            if (patternsizes_[i] == pii{2, 2}) {
                continue;
            }
            if (chessboards.size() >= patternsizes_.size()) {
                break;
            }

            std::vector<cv::Point> corners;
            bool found = cv::findChessboardCorners(mat_, cv::Size{patternsizes_[i].first, patternsizes_[i].second}, corners, \
                cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK);
            if (!found) {
                break;
            }
            markRectArea(mat_, corners);
            chessboards.emplace_back(corners);
        }
    }

    std::sort(chessboards.begin(), chessboards.end(), [](const auto& lhs, const auto& rhs) -> bool {
        return geometry::polygonArea(geometry::getPolygonHull(lhs)) > geometry::polygonArea(geometry::getPolygonHull(rhs));
    });
    while (chessboards.size() > patternsizes_.size()) {
        chessboards.pop_back();
    }
    cornersort::areaCornerSort(chessboards, cameraposition_);

    return chessboards;
}

std::vector<std::vector<cv::Point>> Chessboard::getChessboards(int blocksize, int constantc) {
    std::vector<std::vector<cv::Point>> allchessboards;

    {
        cv::Mat threshmat;
        cv::adaptiveThreshold(mat_, threshmat, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, blocksize, constantc);

        for (int erosion = 0; erosion < 2; ++erosion) {
            cv::dilate(threshmat, threshmat, erosion % 2 == 0 ? crosskernel_ : rectkernel_);

            std::vector<std::vector<cv::Point>> quads = getQuads(threshmat);
            auto chessboards = connectQuads(quads, erosion);

            while (!chessboards.empty()) {
                allchessboards.emplace_back(chessboards.back());
                markRectArea(mat_, chessboards.back());  // atomic<cv::Mat> mat_;
                markRectArea(threshmat, chessboards.back());
                chessboards.pop_back();
            }
        }
    }

    return allchessboards;
}

void Chessboard::markRectArea(cv::Mat& image, const std::vector<cv::Point>& points) {
    const auto [pminx, pmaxx] = std::minmax_element(points.begin(), points.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.x < rhs.x;
    });
    const auto [pminy, pmaxy] = std::minmax_element(points.begin(), points.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.y < rhs.y;
    });
    image(cv::Rect(pminx->x, pminy->y, pmaxx->x - pminx->x, pmaxy->y - pminy->y)) = cv::Scalar(255, 255, 255);
}

std::vector<std::vector<cv::Point>> Chessboard::getQuads(const cv::Mat& threshmat) const {
    auto isValidQuad = [](const std::vector<cv::Point>& quad) -> bool {
        for (int i = 0; i < 2; ++i) {
            cv::Point p1 = quad[(i + 1) % 4] - quad[i];
            cv::Point p2 = quad[(i + 2) % 4] - quad[(i + 3) % 4];
            if (geometry::angle(p1, p2) > 30 / 180.0 * 3.1415926535) {
                return false;
            }
        }
        return true;
    };

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(threshmat, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    int minarea = std::round(threshmat.cols * threshmat.rows * (0.03 * 0.01 * 0.92) * 0.1);

    std::vector<std::vector<cv::Point>> quads;
    for (const auto& contour : contours) {
        std::vector<cv::Point> polygon;
        for (int accuracy = 1; accuracy <= 7; accuracy += 1) {
            cv::approxPolyDP(contour, polygon, cv::arcLength(contour, true) * 0.01 * accuracy, true);
            double polygonarea = cv::contourArea(polygon);
            if (polygon.size() == 4 && minarea <= polygonarea && cv::isContourConvex(polygon) && isValidQuad(polygon)) {
                quads.emplace_back(polygon);
                break;
            }
        }
    }
    
    return quads;
}

std::vector<std::vector<cv::Point>> Chessboard::connectQuads(std::vector<std::vector<cv::Point>>& quads, int erosion) {
    const int n = quads.size();
    
    DSU dsu(n);
    std::vector<std::array<bool, 4>> linked(n, std::array<bool, 4>{false, false, false, false});
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double mindist = std::numeric_limits<double>::max() / 2;
            int pi = -1, pj = -1;
            for (int i1 = 0; i1 < 4; ++i1) {
                if (linked[i][i1]) {
                    continue;
                }

                for (int j1 = 0; j1 < 4; ++j1) {
                    if (linked[j][j1]) {
                        continue;
                    }

                    double dist = geometry::getDistance(cv::Point(quads[i][i1]), cv::Point(quads[j][j1]));
                    if (dist < mindist) {
                        mindist = dist;
                        pi = i1, pj = j1;
                    }
                }
            }

            if (mindist < std::min(quadMinEdgeLength(quads[i]), quadMinEdgeLength(quads[j])) + 2 * erosion \
                && checkLinePosition(quads[i], quads[j], quads[i][pi], quads[j][pj]) \
                && !geometry::polygonInPolygon(quads[i], quads[j])
            ) {
                linked[i][pi] = linked[j][pj] = true;
                quads[i][pi] = quads[j][pj] = (quads[i][pi] + quads[j][pj]) / 2;
                dsu.merge(i, j);
            }
        }
    }

    std::unordered_map<int, int> leaderid;
    std::vector<std::vector<cv::Point>> chessboards;
    // leaderid, linkedcnt, quadcnt
    std::unordered_map<int, std::unordered_map<int, int>> leaderlinkquadcnt;
    for (int i = 0, id = 0; i < n; ++i) {
        int linkedpointcnt = std::count(linked[i].begin(), linked[i].end(), true);
        if (int size = dsu.size(i); \
            !((linkedpointcnt & (linkedpointcnt - 1)) == 0)
            // !(linkedpointcnt == 1 || linkedpointcnt == 2 || linkedpointcnt == 4)
            || !std::any_of(blockcorners_.begin(), blockcorners_.end(), [size](const auto& cur) { return cur.first == size; }) \
        ) {
            continue;
        }

        if (!leaderid.count(dsu.leader(i))) {
            leaderid.emplace(dsu.leader(i), id++);
        }
        while (static_cast<int>(chessboards.size()) <= leaderid.at(dsu.leader(i))) {
            chessboards.emplace_back();
        }
        for (int j = 0; j < 4; ++j) {
            if (linked[i][j]) {
                chessboards[leaderid.at(dsu.leader(i))].emplace_back(quads[i][j]);
            }
        }
        leaderlinkquadcnt[leaderid.at(dsu.leader(i))][linkedpointcnt] += 1;
    }

    auto shouldErase = [this, &leaderlinkquadcnt](const std::vector<std::vector<cv::Point>>::iterator& item, int oldidx) -> bool {
        const int cornercnt = item->size();
        if (!std::any_of(blockcorners_.begin(), blockcorners_.end(), [cornercnt](const auto& cur) { return 2 * cur.second == cornercnt; })) {
            return true;
        }

        assert(cornercnt % 2 == 0);
        cornersort::ChessboardType type = cornersort::ChessboardCornerCntToChessboardType(cornercnt / 2);
        if (type == cornersort::ChessboardType::none) {
            return true;
        }
        if (int linked_one_pointquadcnt = chessboardtypequadinfo[type][1],
            linked_two_pointquadcnt = chessboardtypequadinfo[type][2],
            linked_four_pointquadcnt = chessboardtypequadinfo[type][4];
            !(leaderlinkquadcnt[oldidx][1] == linked_one_pointquadcnt \
            && leaderlinkquadcnt[oldidx][2] == linked_two_pointquadcnt \
            && leaderlinkquadcnt[oldidx][4] == linked_four_pointquadcnt)
        ) {
            return true;
        }

        return false;
    };

    auto eraseForMap = [&leaderid, &leaderlinkquadcnt](int oldidx) {
        for (auto item = leaderid.begin(); item != leaderid.end(); ++item) {
            if (item->second == oldidx) {
                leaderid.erase(item);
                break;
            }
        }
        for (auto item = leaderlinkquadcnt.begin(); item != leaderlinkquadcnt.end(); ++item) {
            if (item->first == oldidx) {
                leaderlinkquadcnt.erase(item);
                break;
            }
        }
    };

    int oldidx = 0;
    for (auto item = chessboards.begin(); item != chessboards.end();) {
        if (shouldErase(item, oldidx)) {
            item = chessboards.erase(item);
            eraseForMap(oldidx);
        } else {
            std::sort(item->begin(), item->end(), [](const auto& lhs, const auto& rhs) {
                return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
            });
            item->erase(std::unique(item->begin(), item->end()), item->end());
            item = std::next(item);
        }

        ++oldidx;
    }

    return chessboards;
}

bool Chessboard::isValid() const {
    if (mat_.empty()) {
        std::printf("error: image.empty()\n");
        return false;
    }
    if (mat_.depth() != CV_8U || mat_.channels() == 2) {
        std::printf("error: image.depth() != CV_8U || image.channels() == 2\n");
        return false;
    }
    if (patternsizes_.empty()) {
        std::printf("error: patternsizes.empty()\n");
        return false;
    }
    if (!std::all_of(patternsizes_.begin(), patternsizes_.end(), [](const auto& cur) {
        return 2 <= cur.first && cur.first <= 4 && 2 <= cur.second && cur.second <= 4;
    })) {
        std::printf("patternsizes out of range: [2, 4]\n");
        return false;
    }
    return true;
}

double Chessboard::quadMinEdgeLength(const std::vector<cv::Point>& quad) const {
    double minedgelen = std::numeric_limits<double>::max() / 2;
    for (int i = 0; i < 4; ++i) {
        minedgelen = std::min(minedgelen, geometry::getDistance(cv::Point(quad[i]), cv::Point(quad[(i + 1) % 4])));
    }
    return minedgelen;
}

bool Chessboard::checkLinePosition(const std::vector<cv::Point>& quada, const std::vector<cv::Point>& quadb, const cv::Point& pa, const cv::Point& pb) const {
    std::array<geometry::Line<int>, 4> lines;
    for (int i = 0; i < 4; ++i) {
        int j = i & 1;
        const auto& quad = i < 2 ? quada : quadb;
        lines[i] = geometry::Line((quad[j] + quad[(j + 1) % 4]) / 2, (quad[(j + 2) % 4] + quad[(j + 3) % 4]) / 2);
    }
    for (const auto& line : lines) {
        if (geometry::pointOnLineLeft(pa, line) != geometry::pointOnLineLeft(pb, line)) {
            return false;
        }
    }
    return true;

    // std::vector<Line<int>> lines(4);
    // for (int i = 0; i < 4; ++i) {
    //     int j = i & 1;
    //     const auto& quad = i < 2 ? quada : quadb;
    //     lines[i] = Line((quad[j] + quad[(j + 1) % 4]) / 2, (quad[(j + 2) % 4] + quad[(j + 3) % 4]) / 2);
    //     auto backup = lines[i];
    //     lines[i].a = (backup.a - backup.b) * 2 + backup.b;
    //     lines[i].b = (backup.b - backup.a) * 2 + backup.a;
    // }
    // return segmentInPolygon(Line{pa, pb}, getPolygonHull(lines));
}
