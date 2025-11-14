#pragma once

#include "common.h"

namespace wak
{

class Timer
{
    using clock = std::chrono::steady_clock;

public:
    WAK_CPU_GPU
    Timer();

    WAK_CPU_GPU
    void Mark();
    WAK_CPU_GPU
    void Reset();

    WAK_CPU_GPU
    double Get();
    WAK_CPU_GPU
    double Elapsed() const;

private:
    std::vector<clock::time_point> time_points;

    size_t ptr;
};

WAK_CPU_GPU inline Timer::Timer()
    : ptr{ 0 }
{
    Mark();
}

WAK_CPU_GPU inline void Timer::Mark()
{
    time_points.push_back(clock::now());
}

WAK_CPU_GPU inline void Timer::Reset()
{
    time_points.clear();
    time_points.push_back(clock::now());
    ptr = 0;
}

WAK_CPU_GPU inline double Timer::Get()
{
    if (ptr < time_points.size() - 1)
    {
        std::chrono::duration<double> dt = time_points[ptr + 1] - time_points[ptr];
        ++ptr;

        return dt.count();
    }
    else
    {
        return 0;
    }
}

WAK_CPU_GPU inline double Timer::Elapsed() const
{
    clock::time_point t = clock::now();
    std::chrono::duration<double> dt = t - time_points.back();
    return dt.count();
}

} // namespace wak
