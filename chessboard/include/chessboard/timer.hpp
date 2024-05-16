#pragma once

#include <chrono>

struct Timer {
    std::chrono::steady_clock::duration starter;
    
    Timer();
    ~Timer();
    double getDuration();
};
