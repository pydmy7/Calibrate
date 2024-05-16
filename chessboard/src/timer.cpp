#include "chessboard/timer.hpp"

#include <cstdio>

Timer::Timer() {
    starter = std::chrono::steady_clock::now().time_since_epoch();
}
Timer::~Timer() {
    std::printf("timer: %.6fs\n", getDuration());
}
double Timer::getDuration() {
    auto ender = std::chrono::steady_clock::now().time_since_epoch();
    double timer = 1E-9 * (ender - starter).count();
    return timer;
}
