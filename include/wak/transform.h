#pragma once

#include "quaternion.h"
#include "vectors.h"

namespace wak
{

struct Transform
{
    Vec3 p; // position
    Quat q; // orientation

    constexpr Transform() = default;

    WAK_CPU_GPU
    constexpr Transform(Identity)
        : p{ 0 }
        , q{ identity }
    {
    }

    WAK_CPU_GPU
    constexpr Transform(const Vec3& position)
        : p{ position }
        , q{ identity }
    {
    }

    WAK_CPU_GPU
    constexpr Transform(const Quat& orientation)
        : p{ 0 }
        , q{ orientation }
    {
    }

    WAK_CPU_GPU
    constexpr Transform(const Vec3& position, const Quat& orientation)
        : p{ position }
        , q{ orientation }
    {
    }

    WAK_CPU_GPU
    constexpr Transform(Float x, Float y, Float z, const Quat& orientation = Quat(1))
        : p{ x, y, z }
        , q{ orientation }
    {
    }

    WAK_CPU_GPU
    Transform(const Mat4& m)
    {
        q = Mat3(Vec3(m[0][0], m[0][1], m[0][2]), Vec3(m[1][0], m[1][1], m[1][2]), Vec3(m[2][0], m[2][1], m[2][2]));

        p.x = m[3][0];
        p.y = m[3][1];
        p.z = m[3][2];
    }

    WAK_CPU_GPU
    constexpr void Set(const Vec3& position, const Quat& orientation)
    {
        p = position;
        q = orientation;
    }

    WAK_CPU_GPU
    constexpr void SetIdentity()
    {
        p.SetZero();
        q.SetIdentity();
    }

    std::string ToString() const
    {
        return std::format("p:{}\nq:{}", p.ToString(), q.ToString());
    }

    WAK_CPU_GPU
    constexpr Transform& operator*=(const Transform& other);

    WAK_CPU_GPU
    constexpr Transform GetInverse() const
    {
        Quat inv_q = q.GetConjugate();
        return Transform{ inv_q.Rotate(-p), inv_q };
    }

    WAK_CPU_GPU
    static Transform Translate(const Vec3& position)
    {
        return Transform(position);
    }

    WAK_CPU_GPU
    static Transform Rotate(const Vec3& rotation)
    {
        return Transform(Quat::FromEuler(rotation));
    }

    WAK_CPU_GPU
    static Transform LookAt(const Vec3& position, const Vec3& target, const Vec3& up)
    {
        Vec3 w = target - position;
        w.Normalize();
        return Transform(position, Quat(w, up));
    }
};

WAK_CPU_GPU constexpr inline bool operator==(const Transform& a, const Transform& b)
{
    return a.p == b.p && a.q == b.q;
}

WAK_CPU_GPU constexpr inline Vec3 operator*(const Transform& t, const Vec3& v)
{
    return t.q.Rotate(v) + t.p;
}

// A * V
WAK_CPU_GPU constexpr inline Vec3 Mul(const Transform& t, const Vec3& v)
{
    return t.q.Rotate(v) + t.p;
}

// A^{-1} * V
WAK_CPU_GPU constexpr inline Vec3 MulT(const Transform& t, const Vec3& v)
{
    return t.q.RotateInv(v - t.p);
}

WAK_CPU_GPU constexpr inline Transform operator*(const Transform& a, const Transform& b)
{
    return Transform{ a.q.Rotate(b.p) + a.p, a.q * b.q };
}

// A * B
WAK_CPU_GPU constexpr inline Transform Mul(const Transform& a, const Transform& b)
{
    return Transform{ a.q.Rotate(b.p) + a.p, a.q * b.q };
}

// A^{-1} * B
WAK_CPU_GPU constexpr inline Transform MulT(const Transform& a, const Transform& b)
{
    Quat inv_q = a.q.GetConjugate();
    return Transform{ inv_q.Rotate(b.p - a.p), inv_q * b.q };
}

WAK_CPU_GPU constexpr inline Transform& Transform::operator*=(const Transform& other)
{
    *this = Mul(*this, other);
    return *this;
}

} // namespace wak
