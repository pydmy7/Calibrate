#include "config.h"

#include "chessboard/chessboard.hpp"
#include "chessboard/cornersort.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <spdlog/spdlog.h>

#include <unordered_map>
#include <filesystem>
#include <string>
#include <initializer_list>
#include <utility>

namespace fs = std::filesystem;

const std::unordered_map<int, cornersort::CameraPosition> cameraposition = {
    {0, cornersort::CameraPosition::front},
    {1, cornersort::CameraPosition::frontright},
    {2, cornersort::CameraPosition::rearright},
    {3, cornersort::CameraPosition::rear},
    {4, cornersort::CameraPosition::rearleft},
    {5, cornersort::CameraPosition::frontleft}
};

const fs::path sourceroot {SOURCE_DIR};
const fs::path inputroot {sourceroot / "images"};
const fs::path outputroot {sourceroot / "output"};

void testAllImage() {
    auto solve = [](std::string&& imagepath, std::string&& outputpath) -> void {
        cv::Mat image = cv::imread(imagepath);
        std::initializer_list<std::pair<int, int>> patternsize =
            imagepath.find("3-3") != std::string::npos
            ? std::initializer_list<std::pair<int, int>>{
                std::pair<int, int>{3, 3},
                std::pair<int, int>{3, 3}
            }
            : std::initializer_list<std::pair<int, int>>{
                std::pair<int, int>{2, 2},
                std::pair<int, int>{2, 2},
                std::pair<int, int>{3, 4}
            };
        cornersort::CameraPosition campos = cameraposition.at(0);

        if (patternsize.size() == 2) {  // 3-3
            auto pos = imagepath.find("src");
            if (pos == std::string::npos) {
                assert(false);
                return;
            }
            auto lpos {pos + 3}, rpos {lpos + 1};
            while (rpos < imagepath.size() && std::isdigit(imagepath[rpos])) {
                ++rpos;
            }
            int idx {std::stoi(imagepath.substr(lpos, rpos - lpos))};
            campos = cameraposition.at(idx);
        }

        Chessboard cur(image, patternsize, campos);
        std::vector<std::vector<cv::Point>> chessboards = cur.getChessboards();

        for (int idx = 0; auto&& corners : chessboards) {
            for (int j = 0; j < static_cast<int>(corners.size()); ++j) {
                cv::putText(image, std::to_string(idx++), corners[j], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
                // std::cout << corners[j] << " \n"[j == corners.size() - 1];
            }
        }

        cv::imwrite(outputpath, image);
    };

    for (auto&& entry  : fs::recursive_directory_iterator(inputroot)) {
        if (!fs::is_regular_file(entry))  {
            continue;
        }
        fs::create_directories(outputroot / fs::relative(entry.path(), inputroot).parent_path());
        solve(entry.path().string(), (outputroot / fs::relative(entry.path(), inputroot)).string());
    }
}

int main() {

    testAllImage();

    return 0;
}
