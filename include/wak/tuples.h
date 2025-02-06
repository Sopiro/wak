#pragma once

#include "asserts.h"
#include "floats.h"
#include "format.h"
#include "types.h"

namespace wak
{

template <template <typename> class Child, typename T>
struct Tuple2
{
    static const int32 dimensions = 2;

    constexpr Tuple2() = default;

    WAK_CPU_GPU
    constexpr Tuple2(T x, T y)
        : x{ x }
        , y{ y }
    {
        // WakAssert(!IsNullish());
    }

    WAK_CPU_GPU
    bool IsNullish() const
    {
        return wak::IsNullish(x) || wak::IsNullish(y);
    }

    WAK_CPU_GPU
    T operator[](int32 i) const
    {
        WakAssert(i >= 0 && i <= 1);
        return (&x)[i];
    }

    WAK_CPU_GPU
    T& operator[](int32 i)
    {
        WakAssert(i >= 0 && i <= 1);
        return (&x)[i];
    }

    WAK_CPU_GPU
    constexpr bool operator==(Child<T> c) const
    {
        return x == c.x && y == c.y;
    }

    WAK_CPU_GPU
    constexpr bool operator!=(Child<T> c) const
    {
        return x != c.x || y != c.y;
    }

    WAK_CPU_GPU constexpr Child<T> operator-() const
    {
        return { -x, -y };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator+(Child<U> c) const -> Child<decltype(T{} + U{})>
    {
        WakAssert(!c.IsNullish());
        return { x + c.x, y + c.y };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator-(Child<U> c) const -> Child<decltype(T{} - U{})>
    {
        WakAssert(!c.IsNullish());
        return { x - c.x, y - c.y };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator*(U s) const -> Child<decltype(T{} * U{})>
    {
        return { s * x, s * y };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator/(U d) const -> Child<decltype(T{} / U{})>
    {
        WakAssert(d != 0 && !wak::IsNullish(d));
        return { x / d, y / d };
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator+=(Child<U> c)
    {
        WakAssert(!c.IsNullish());
        x += c.x;
        y += c.y;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator-=(Child<U> c)
    {
        WakAssert(!c.IsNullish());
        x -= c.x;
        y -= c.y;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator*=(U s)
    {
        WakAssert(!wak::IsNullish(s));
        x *= s;
        y *= s;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator/=(U d)
    {
        WakAssert(d != 0);
        WakAssert(!wak::IsNullish(d));
        x /= d;
        y /= d;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator*=(Child<U> s)
    {
        WakAssert(!wak::IsNullish(s));
        x *= s.x;
        y *= s.y;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator/=(Child<U> d)
    {
        WakAssert(d != 0);
        WakAssert(!wak::IsNullish(d));
        x /= d.x;
        y /= d.y;
        return static_cast<Child<T>&>(*this);
    }

    WAK_CPU_GPU
    constexpr void SetZero()
    {
        x = T(0);
        y = T(0);
    }

    WAK_CPU_GPU
    constexpr void Set(T nx, T ny)
    {
        x = nx;
        y = ny;
    }

    std::string ToString() const
    {
        return FormatString("%.4f\t%.4f", x, y);
    }

    T x{}, y{};
};

template <template <typename> class Child, typename T>
struct Tuple3
{
    static const int32 dimensions = 3;

    constexpr Tuple3() = default;

    WAK_CPU_GPU
    constexpr Tuple3(T x, T y, T z)
        : x{ x }
        , y{ y }
        , z{ z }
    {
        // WakAssert(!IsNullish());
    }

    WAK_CPU_GPU
    bool IsNullish() const
    {
        return wak::IsNullish(x) || wak::IsNullish(y) || wak::IsNullish(z);
    }

    WAK_CPU_GPU
    T operator[](int32 i) const
    {
        WakAssert(i >= 0 && i <= 2);
        return (&x)[i];
    }

    WAK_CPU_GPU
    T& operator[](int32 i)
    {
        WakAssert(i >= 0 && i <= 2);
        return (&x)[i];
    }

    WAK_CPU_GPU
    constexpr bool operator==(Child<T> c) const
    {
        return x == c.x && y == c.y && z == c.z;
    }

    WAK_CPU_GPU
    constexpr bool operator!=(Child<T> c) const
    {
        return x != c.x || y != c.y || z != c.z;
    }

    WAK_CPU_GPU
    constexpr Child<T> operator-() const
    {
        return { -x, -y, -z };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator+(Child<U> c) const -> Child<decltype(T{} + U{})>
    {
        WakAssert(!c.IsNullish());
        return { x + c.x, y + c.y, z + c.z };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator-(Child<U> c) const -> Child<decltype(T{} - U{})>
    {
        WakAssert(!c.IsNullish());
        return { x - c.x, y - c.y, z - c.z };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator*(U s) const -> Child<decltype(T{} * U{})>
    {
        return { s * x, s * y, s * z };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator/(U d) const -> Child<decltype(T{} / U{})>
    {
        WakAssert(d != 0);
        return { x / d, y / d, z / d };
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator+=(Child<U> c)
    {
        WakAssert(!c.IsNullish());
        x += c.x;
        y += c.y;
        z += c.z;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator-=(Child<U> c)
    {
        WakAssert(!c.IsNullish());
        x -= c.x;
        y -= c.y;
        z -= c.z;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator*=(U s)
    {
        WakAssert(!wak::IsNullish(s));
        x *= s;
        y *= s;
        z *= s;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator/=(U d)
    {
        WakAssert(d != 0);
        x /= d;
        y /= d;
        z /= d;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator*=(Child<U> s)
    {
        WakAssert(!wak::IsNullish(s));
        x *= s.x;
        y *= s.y;
        z *= s.z;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator/=(Child<U> d)
    {
        WakAssert(d != 0);
        x /= d.x;
        y /= d.y;
        z /= d.z;
        return static_cast<Child<T>&>(*this);
    }

    WAK_CPU_GPU
    constexpr void SetZero()
    {
        x = T(0);
        y = T(0);
        z = T(0);
    }

    WAK_CPU_GPU
    constexpr void Set(T nx, T ny, T nz)
    {
        x = nx;
        y = ny;
        z = nz;
    }

    std::string ToString() const
    {
        return FormatString("%.4f\t%.4f\t%.4f", x, y, z);
    }

    T x{}, y{}, z{};
};

template <template <typename> class Child, typename T>
struct Tuple4
{
    static const int32 dimensions = 4;

    constexpr Tuple4() = default;

    WAK_CPU_GPU
    constexpr Tuple4(T x, T y, T z, T w)
        : x{ x }
        , y{ y }
        , z{ z }
        , w{ w }
    {
        // WakAssert(!IsNullish());
    }

    WAK_CPU_GPU
    bool IsNullish() const
    {
        return wak::IsNullish(x) || wak::IsNullish(y) || wak::IsNullish(z) || wak::IsNullish(w);
    }

    WAK_CPU_GPU
    T operator[](int32 i) const
    {
        WakAssert(i >= 0 && i <= 3);
        return (&x)[i];
    }

    WAK_CPU_GPU
    T& operator[](int32 i)
    {
        WakAssert(i >= 0 && i <= 3);
        return (&x)[i];
    }

    WAK_CPU_GPU
    constexpr bool operator==(Child<T> c) const
    {
        return x == c.x && y == c.y && z == c.z && w == c.w;
    }

    WAK_CPU_GPU
    constexpr bool operator!=(Child<T> c) const
    {
        return x != c.x || y != c.y || z != c.z || w != c.w;
    }

    WAK_CPU_GPU
    constexpr Child<T> operator-() const
    {
        return { -x, -y, -z, -w };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator+(Child<U> c) const -> Child<decltype(T{} + U{})>
    {
        WakAssert(!c.IsNullish());
        return { x + c.x, y + c.y, z + c.z, w + c.w };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator-(Child<U> c) const -> Child<decltype(T{} - U{})>
    {
        WakAssert(!c.IsNullish());
        return { x - c.x, y - c.y, z - c.z, z - c.w };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator*(U s) const -> Child<decltype(T{} * U{})>
    {
        return { s * x, s * y, s * z, s * w };
    }

    template <typename U>
    WAK_CPU_GPU constexpr auto operator/(U d) const -> Child<decltype(T{} / U{})>
    {
        WakAssert(d != 0);
        return { x / d, y / d, z / d, w / d };
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator+=(Child<U> c)
    {
        WakAssert(!c.IsNullish());
        x += c.x;
        y += c.y;
        z += c.z;
        w += c.w;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator-=(Child<U> c)
    {
        WakAssert(!c.IsNullish());
        x -= c.x;
        y -= c.y;
        z -= c.z;
        w -= c.w;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator*=(U s)
    {
        WakAssert(!wak::IsNullish(s));
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator/=(U d)
    {
        WakAssert(d != 0);
        x /= d;
        y /= d;
        z /= d;
        w /= d;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator*=(Child<U> s)
    {
        WakAssert(!wak::IsNullish(s));
        x *= s.x;
        y *= s.y;
        z *= s.z;
        w *= s.w;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    WAK_CPU_GPU constexpr Child<T>& operator/=(Child<U> d)
    {
        WakAssert(d != 0);
        x /= d.x;
        y /= d.y;
        z /= d.z;
        w /= d.w;
        return static_cast<Child<T>&>(*this);
    }

    WAK_CPU_GPU
    constexpr void SetZero()
    {
        x = T(0);
        y = T(0);
        z = T(0);
        w = T(0);
    }

    WAK_CPU_GPU
    constexpr void Set(T nx, T ny, T nz, T nw)
    {
        x = nx;
        y = ny;
        z = nz;
        w = nw;
    }

    std::string ToString() const
    {
        return FormatString("%.4f\t%.4f\t%.4f\t%.4f", x, y, z, w);
    }

    T x{}, y{}, z{}, w{};
};

} // namespace wak
