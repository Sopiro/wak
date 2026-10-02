#pragma once

#include <assert.h>           // IWYU pragma: export
#include <chrono>             // IWYU pragma: export
#include <cmath>              // IWYU pragma: export
#include <condition_variable> // IWYU pragma: export
#include <cstdint>            // IWYU pragma: export
#include <cstring>            // IWYU pragma: export
#include <filesystem>         // IWYU pragma: export
#include <functional>         // IWYU pragma: export
#include <iostream>           // IWYU pragma: export
#include <latch>              // IWYU pragma: export
#include <mutex>              // IWYU pragma: export
#include <optional>           // IWYU pragma: export
#include <shared_mutex>       // IWYU pragma: export
#include <thread>             // IWYU pragma: export

#include "types.h"            // IWYU pragma: export

#ifdef __CUDACC__
#define WAK_GPU __device__
#define WAK_CPU_GPU __host__ __device__
#else
#define WAK_GPU
#define WAK_CPU_GPU
#endif

#define WakAssert(A) assert(A)
#define WakNotUsed(x) ((void)(x))