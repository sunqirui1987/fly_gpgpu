#pragma once

#include <chrono>
#include <cstdint>

namespace fly
{

class Timer
{
public:
    void start();
    void stop();
    uint64_t getElapsedTime() const;

private:
    std::chrono::steady_clock::time_point initialTime;
    std::chrono::steady_clock::time_point endTime;
};

} // namespace fly
