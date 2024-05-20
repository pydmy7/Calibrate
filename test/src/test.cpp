#include "chessboard/chessboard.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <spdlog/spdlog.h>

#include <iostream>
#include <string>
#include <unordered_map>

using namespace std::string_literals;

// this is a test and usage demo

std::unordered_map<int, cornersort::CameraPosition> cameraposition = {
    {0, cornersort::CameraPosition::front},
    {1, cornersort::CameraPosition::frontright},
    {2, cornersort::CameraPosition::rearright},
    {3, cornersort::CameraPosition::rear},
    {4, cornersort::CameraPosition::rearleft},
    {5, cornersort::CameraPosition::frontleft}
};

void testWeiChai() {
    for (int group = 5; group >= 0; --group) {
        for (int i = 0; i < 6; ++i) {
            // std::string filepath = std::format("{}/src{}.png", group, i);
            std::string filepath = "images/3-3/weichai/" + std::to_string(group) + "/src" + std::to_string(i) + ".png";
            cv::Mat image = cv::imread(filepath);
            if (image.empty()) {
                std::cout << "image empty\n";
                return;
            }

            Chessboard cur{image, {{3, 3}, {3, 3}}, cameraposition[i]};
            std::vector<std::vector<cv::Point>> chessboards = cur.getChessboards();
            [[maybe_unused]] bool found = !chessboards.empty();

            int idx = 0;
            for (const auto& corners : chessboards) {
                for (int j = 0; j < static_cast<int>(corners.size()); ++j) {
                    cv::putText(image, std::to_string(idx++), corners[j], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
                    // std::cout << corners[j] << " \n"[j == corners.size() - 1];
                }
            }
            // cv::imshow(filepath, image);
            // cv::waitKey(1E3);
            // cv::destroyWindow(filepath);
            std::cout << "output/3-3/weichai/" + std::to_string(group) + "/" + std::to_string(i) + ".png" << '\n';
            cv::imwrite("output/3-3/weichai/" + std::to_string(group) + "/" + std::to_string(i) + ".png", image);
        }
    }
}

void testJieFang() {
    for (int group = 4; group >= 4; --group) {
        for (int i = 0; i < 6; ++i) {
            // std::string filepath = std::format("{}/src{}.png", group, i);
            std::string filepath = "images/3-3/jiefang/" + std::to_string(group) + "/src" + std::to_string(i) + ".png";
            cv::Mat image = cv::imread(filepath);
            if (image.empty()) {
                std::cout << "image empty\n";
                return;
            }

            Chessboard cur(image, {{3, 3}, {3, 3}}, cameraposition[i]);
            std::vector<std::vector<cv::Point>> chessboards = cur.getChessboards();
            [[maybe_unused]] bool found = !chessboards.empty();

            int idx = 0;
            for (const auto& corners : chessboards) {
                for (int j = 0; j < static_cast<int>(corners.size()); ++j) {
                    cv::putText(image, std::to_string(idx++), corners[j], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
                    std::cout << corners[j] << " \n"[j == static_cast<int>(corners.size()) - 1];
                }
            }
            cv::imshow(filepath, image);
            cv::waitKey(0);
            cv::destroyWindow(filepath);
            // std::cout << "output/3-3/jiefang/" + std::to_string(group) + "/" + std::to_string(i) + ".png" << '\n';
            // cv::imwrite("output/3-3/jiefang/" + std::to_string(group) + "/" + std::to_string(i) + ".png", image);
        }
    }
}

void test34() {
    for (int i = 0; i < 32; ++i) {
        // std::string filepath = std::format("images/3-4/src{}.png", i);
        std::string filepath = "images/3-4/"s + "src"s + std::to_string(i) + ".png";
        cv::Mat image = cv::imread(filepath);
        if (image.empty()) {
            std::cout << "image empty\n";
            return;
        }

        std::array<cornersort::CameraPosition, 4> AAA = {
            cornersort::CameraPosition::front,
            cornersort::CameraPosition::right,
            cornersort::CameraPosition::rear,
            cornersort::CameraPosition::left
        };

        Chessboard cur(image, {{3, 4}}, AAA[i % 4]);
        std::vector<std::vector<cv::Point>> chessboards = cur.getChessboards();
        [[maybe_unused]] bool found = !chessboards.empty();

        int idx = 0;
        for (const auto& corners : chessboards) {
            for (int j = 0; j < static_cast<int>(corners.size()); ++j) {
                cv::putText(image, std::to_string(idx++), corners[j], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
                std::cout << corners[j] << " \n"[j == static_cast<int>(corners.size()) - 1];
            }
        }
        // cv::imshow(filepath, image);
        // cv::waitKey(0);
        // cv::destroyWindow(filepath);
        std::cout << "output/3-4/" + std::to_string(i) + ".png" << '\n';
        cv::imwrite("output/3-4/" + std::to_string(i) + ".png", image);
    }
}

void test() {
    for (int i = 0; i < 6; ++i) {
        std::string filepath = "images/3-3/jiefang/0/src" +  std::to_string(i) + ".png";
        cv::Mat image = cv::imread(filepath);
        if (image.empty()) {
            std::cout << "image empty\n";
            return;
        }

        Chessboard cur(image, {{3, 3}, {3, 3}}, cameraposition[i]);
        std::vector<std::vector<cv::Point>> chessboards = cur.getChessboards();
        [[maybe_unused]] bool found = !chessboards.empty();

        int idx = 0;
        for (const auto& corners : chessboards) {
            for (int j = 0; j < static_cast<int>(corners.size()); ++j) {
                cv::putText(image, std::to_string(idx++), corners[j], cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
                std::cout << corners[j] << " \n"[j == static_cast<int>(corners.size()) - 1];
            }
        }
        cv::imshow(filepath, image);
        cv::waitKey(0);
        cv::destroyWindow(filepath);
    }
}

int main() {

    // test34();
    testWeiChai();
    // testJieFang();
    // test();

    spdlog::info("Hello, {}!", "world");
    spdlog::warn("This is a warning!");
    spdlog::error("This is an error!");
    spdlog::critical("Critical error with number: {}", 42);
    spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e %l %n %v");
    spdlog::info("Custom pattern log example");

    return 0;
}
