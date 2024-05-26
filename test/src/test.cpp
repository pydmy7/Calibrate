#include "chessboard/chessboard.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <spdlog/spdlog.h>

#include <iostream>
#include <unordered_map>
#include <filesystem>

std::unordered_map<int, cornersort::CameraPosition> cameraposition = {
    {0, cornersort::CameraPosition::front},
    {1, cornersort::CameraPosition::frontright},
    {2, cornersort::CameraPosition::rearright},
    {3, cornersort::CameraPosition::rear},
    {4, cornersort::CameraPosition::rearleft},
    {5, cornersort::CameraPosition::frontleft}
};

constexpr std::string_view sourcedir = SOURCE_DIR;

void testWeiChai() {
    for (int group = 5; group >= 0; --group) {
        for (int i = 0; i < 6; ++i) {
            std::string inputfilepath = fmt::format("{}/images/3-3/weichai/{}/src{}.png", sourcedir, group, i);
            cv::Mat image = cv::imread(inputfilepath);
            if (image.empty()) {
                std::cerr << "image empty\n";
                return;
            }

            Chessboard cur{image, {{3, 3}, {3, 3}}, cameraposition[i]};
            std::vector<std::vector<cv::Point>> chessboards = cur.getChessboards();
            [[maybe_unused]] bool found = !chessboards.empty();

            for (int idx = 0; auto&& corners : chessboards) {
                for (int j = 0; j < static_cast<int>(corners.size()); ++j) {
                    cv::putText(image, std::to_string(idx++), corners[j], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
                    // std::cout << corners[j] << " \n"[j == corners.size() - 1];
                }
            }

            std::string outputdirpath = fmt::format("{}/output/3-3/weichai/{}", sourcedir, group);
            std::string outputfilepath = fmt::format("{}/src{}.png", outputdirpath, i);
            std::filesystem::create_directories(outputdirpath);
            cv::imwrite(outputfilepath, image);
            std::clog << outputfilepath << '\n';
        }
    }
}

void testJieFang() {
    for (int group = 4; group >= 0; --group) {
        for (int i = 0; i < 6; ++i) {
            std::string inputfilepath = fmt::format("{}/images/3-3/jiefang/{}/src{}.png", sourcedir, group, i);
            cv::Mat image = cv::imread(inputfilepath);
            if (image.empty()) {
                std::cerr << "image empty\n";
                return;
            }

            Chessboard cur{image, {{3, 3}, {3, 3}}, cameraposition[i]};
            std::vector<std::vector<cv::Point>> chessboards = cur.getChessboards();
            [[maybe_unused]] bool found = !chessboards.empty();

            for (int idx = 0; auto&& corners : chessboards) {
                for (int j = 0; j < static_cast<int>(corners.size()); ++j) {
                    cv::putText(image, std::to_string(idx++), corners[j], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
                    // std::cout << corners[j] << " \n"[j == corners.size() - 1];
                }
            }

            std::string outputdirpath = fmt::format("{}/output/3-3/jiefang/{}", sourcedir, group);
            std::string outputfilepath = fmt::format("{}/src{}.png", outputdirpath, i);
            std::filesystem::create_directories(outputdirpath);
            cv::imwrite(outputfilepath, image);
            std::clog << outputfilepath << '\n';
        }
    }
}

void test34() {
    for (int i = 31; i >= 0; --i) {
        std::string inputfilepath = fmt::format("{}/images/3-4/src{}.png", sourcedir, i);
        cv::Mat image = cv::imread(inputfilepath);
        if (image.empty()) {
            std::cerr << "image empty\n";
            return;
        }

        Chessboard cur{image, {{2, 2}, {2, 2}, {3, 4}}, cameraposition[i]};
        std::vector<std::vector<cv::Point>> chessboards = cur.getChessboards();
        [[maybe_unused]] bool found = !chessboards.empty();

        for (int idx = 0; auto&& corners : chessboards) {
            for (int j = 0; j < static_cast<int>(corners.size()); ++j) {
                cv::putText(image, std::to_string(idx++), corners[j], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
                // std::cout << corners[j] << " \n"[j == corners.size() - 1];
            }
        }

        std::string outputdirpath = fmt::format("{}/output/3-4", sourcedir);
        std::string outputfilepath = fmt::format("{}/src{}.png", outputdirpath, i);
        std::filesystem::create_directories(outputdirpath);
        cv::imwrite(outputfilepath, image);
        std::clog << outputfilepath << '\n';
    }
}

int main() {

    testWeiChai();
    testJieFang();
    test34();

    spdlog::info("Hello, {}!", "world");
    spdlog::warn("This is a warning!");
    spdlog::error("This is an error!");
    spdlog::critical("Critical error with number: {}", 42);
    spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e %l %n %v");
    spdlog::info("Custom pattern log example");

    return 0;
}
