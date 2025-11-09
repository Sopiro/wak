#pragma once

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <functional>
#include <iostream>
#include <latch>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <thread>

#ifdef __CUDACC__
#define WAK_GPU __device__
#define WAK_CPU_GPU __host__ __device__
#else
#define WAK_GPU
#define WAK_CPU_GPU
#endif

template <typename T>
WAK_CPU_GPU inline void Swap(T& a, T& b)
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}
