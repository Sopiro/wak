#pragma once

#include "asserts.h"
#include "common.h"
#include "types.h"

namespace wak
{

#ifdef WAK_DOUBLE_PRECISION
using Float = double;
#else
using Float = float;
#endif

constexpr Float pi = Float(3.14159265358979323846);
constexpr Float two_pi = Float(2 * pi);
constexpr Float four_pi = Float(4 * pi);
constexpr Float inv_pi = Float(1 / pi);
constexpr Float inv_two_pi = Float(1 / (2 * pi));
constexpr Float inv_four_pi = Float(1 / (4 * pi));
constexpr Float epsilon = std::numeric_limits<Float>::epsilon();
constexpr Float infinity = std::numeric_limits<Float>::infinity();
constexpr Float max_float = std::numeric_limits<Float>::max();

WAK_CPU_GPU
inline bool IsNullish(int32 v)
{
    WakNotUsed(v);
    return false;
}

WAK_CPU_GPU
inline bool IsNullish(Float v)
{
    return std::isnan(v) || std::isinf(v);
}

WAK_CPU_GPU
template <typename T>
inline bool IsNullish(const T& v)
{
    return v.IsNullish();
}

#define CheckNull(v)                                                                                                             \
    if (IsNullish(v))                                                                                                            \
    {                                                                                                                            \
        std::cout << #v;                                                                                                         \
        std::cout << " null" << std::endl;                                                                                       \
        WakAssert(false);                                                                                                        \
    }

} // namespace wak
