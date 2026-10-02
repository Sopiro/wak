#pragma once

#include "matrix.h"
#include "vectors.h"

namespace wak
{

inline Float Length(const Quat& q);
inline constexpr Quat operator*(const Quat& q, Float s);
inline constexpr Quat operator*(Float s, const Quat& q);

struct Quat
{
    Float x, y, z, w;

    constexpr Quat() = default;

    WAK_CPU_GPU
    constexpr Quat(Identity)
        : Quat(1)
    {
    }

    WAK_CPU_GPU
    constexpr Quat(Vec3 v, Float w)
        : x{ v.x }
        , y{ v.y }
        , z{ v.z }
        , w{ w }
    {
    }

    WAK_CPU_GPU
    constexpr Quat(Float x, Float y, Float z, Float w)
        : x{ x }
        , y{ y }
        , z{ z }
        , w{ w }
    {
    }

    WAK_CPU_GPU
    explicit constexpr Quat(Float w)
        : x{ 0 }
        , y{ 0 }
        , z{ 0 }
        , w{ w }
    {
    }

    WAK_CPU_GPU
    Quat(const Mat3& m)
    {
        // https://math.stackexchange.com/questions/893984/conversion-of-rotation-matrix-to-quaternion
        if (m.ez.z < 0)
        {
            if (m.ex.x > m.ey.y)
            {
                Float t = 1 + m.ex.x - m.ey.y - m.ez.z;
                *this = Quat(t, m.ex.y + m.ey.x, m.ez.x + m.ex.z, m.ey.z - m.ez.y) * (0.5f / std::sqrt(t));
            }
            else
            {
                Float t = 1 - m.ex.x + m.ey.y - m.ez.z;
                *this = Quat(m.ex.y + m.ey.x, t, m.ey.z + m.ez.y, m.ez.x - m.ex.z) * (0.5f / std::sqrt(t));
            }
        }
        else
        {
            if (m.ex.x < -m.ey.y)
            {
                Float t = 1 - m.ex.x - m.ey.y + m.ez.z;
                *this = Quat(m.ez.x + m.ex.z, m.ey.z + m.ez.y, t, m.ex.y - m.ey.x) * (0.5f / std::sqrt(t));
            }
            else
            {
                Float t = 1 + m.ex.x + m.ey.y + m.ez.z;
                *this = Quat(m.ey.z - m.ez.y, m.ez.x - m.ex.z, m.ex.y - m.ey.x, t) * (0.5f / std::sqrt(t));
            }
        }
    }

    WAK_CPU_GPU
    Quat(const Vec3& front, const Vec3& up)
    {
        Mat3 rotation;

        rotation.ez = front;
        rotation.ex = Cross(up, rotation.ez);
        rotation.ex.Normalize();
        rotation.ey = Cross(rotation.ez, rotation.ex);

        *this = Quat(rotation);
    }

    // Axis must be normalized
    WAK_CPU_GPU
    Quat(Float angle, const Vec3& unit_axis)
    {
        Float half_angle = angle * 0.5f;

        Float s = std::sin(half_angle);
        x = unit_axis.x * s;
        y = unit_axis.y * s;
        z = unit_axis.z * s;
        w = std::cos(half_angle);
    }

    WAK_CPU_GPU
    constexpr Quat operator-() const
    {
        return Quat(-x, -y, -z, -w);
    }

    WAK_CPU_GPU
    constexpr Quat& operator+=(Quat q)
    {
        x += q.x;
        y += q.y;
        z += q.z;
        w += q.w;
        return *this;
    }

    WAK_CPU_GPU
    constexpr Quat& operator-=(Quat q)
    {
        x -= q.x;
        y -= q.y;
        z -= q.z;
        w -= q.w;
        return *this;
    }

    WAK_CPU_GPU
    constexpr Quat& operator*=(Float s)
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }

    WAK_CPU_GPU
    constexpr Quat& operator/=(Float s)
    {
        WakAssert(s != 0);
        Float invS = 1 / s;
        x *= invS;
        y *= invS;
        z *= invS;
        w *= invS;
        return *this;
    }

    WAK_CPU_GPU
    Float Normalize()
    {
        Float length = Length(*this);
        if (length < std::numeric_limits<Float>::epsilon())
        {
            return Float(0);
        }

        Float invLength = Float(1) / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;
        w *= invLength;

        return length;
    }

    WAK_CPU_GPU
    constexpr bool IsIdentity() const
    {
        return x == 0 && y == 0 && z == 0 && w == 1;
    }

    WAK_CPU_GPU
    constexpr void SetIdentity()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 1;
    }

    WAK_CPU_GPU
    constexpr Quat GetConjugate() const
    {
        return Quat(-x, -y, -z, w);
    }

    WAK_CPU_GPU
    constexpr Vec3 GetImaginaryPart() const
    {
        return Vec3(x, y, z);
    }

    // Optimized qvq'
    WAK_CPU_GPU
    constexpr Vec3 Rotate(const Vec3& v) const
    {
        Float vx = 2 * v.x;
        Float vy = 2 * v.y;
        Float vz = 2 * v.z;
        Float w2 = w * w - 0.5f;

        Float dot2 = (x * vx + y * vy + z * vz);

        return Vec3{
            vx * w2 + (y * vz - z * vy) * w + x * dot2,
            vy * w2 + (z * vx - x * vz) * w + y * dot2,
            vz * w2 + (x * vy - y * vx) * w + z * dot2,
        };
    }

    WAK_CPU_GPU
    constexpr Vec3 RotateInv(const Vec3& v) const
    {
        Float vx = 2 * v.x;
        Float vy = 2 * v.y;
        Float vz = 2 * v.z;
        Float w2 = w * w - 0.5f;

        Float dot2 = (x * vx + y * vy + z * vz);

        return Vec3{
            vx * w2 - (y * vz - z * vy) * w + x * dot2,
            vy * w2 - (z * vx - x * vz) * w + y * dot2,
            vz * w2 - (x * vy - y * vx) * w + z * dot2,
        };
    }

    // Computes rotation of x-axis
    WAK_CPU_GPU
    constexpr Vec3 GetBasisX() const
    {
        Float x2 = x * 2;
        Float w2 = w * 2;

        return Vec3((w * w2) - 1 + x * x2, (z * w2) + y * x2, (-y * w2) + z * x2);
    }

    // Computes rotation of y-axis
    WAK_CPU_GPU
    constexpr Vec3 GetBasisY() const
    {
        Float y2 = y * 2;
        Float w2 = w * 2;

        return Vec3((-z * w2) + x * y2, (w * w2) - 1 + y * y2, (x * w2) + z * y2);
    }

    // Computes rotation of z-axis
    WAK_CPU_GPU
    constexpr Vec3 GetBasisZ() const
    {
        Float z2 = z * 2;
        Float w2 = w * 2;

        return Vec3((y * w2) + x * z2, (-x * w2) + y * z2, (w * w2) - 1 + z * z2);
    }

    WAK_CPU_GPU
    Vec3 ToEuler() const
    {
        // Roll (x-axis)
        Float sinr_cosp = 2 * (w * x + y * z);
        Float cosr_cosp = 1 - 2 * (x * x + y * y);
        Float roll = std::atan2(sinr_cosp, cosr_cosp);

        // Pitch (y-axis)
        Float sinp = 2 * (w * y - z * x);
        Float pitch;
        if (std::abs(sinp) >= 1)
        {
            pitch = std::copysign(pi / 2, sinp); // use 90 degrees if out of range
        }
        else
        {
            pitch = std::asin(sinp);
        }

        // Yaw (z-axis)
        Float siny_cosp = 2 * (w * z + x * y);
        Float cosy_cosp = 1 - 2 * (y * y + z * z);
        Float yaw = std::atan2(siny_cosp, cosy_cosp);

        return Vec3{ roll, pitch, yaw };
    }

    WAK_CPU_GPU
    static Quat FromEuler(Float x, Float y, Float z)
    {
        Float cr = std::cos(x * 0.5f);
        Float sr = std::sin(x * 0.5f);
        Float cp = std::cos(y * 0.5f);
        Float sp = std::sin(y * 0.5f);
        Float cy = std::cos(z * 0.5f);
        Float sy = std::sin(z * 0.5f);

        Quat q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;

        return q;
    }

    WAK_CPU_GPU
    static Quat FromEuler(const Vec3& eulerAngles)
    {
        return FromEuler(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    }

    std::string ToString() const
    {
        return ToEuler().ToString();
    }

    static const Quat zero;
};

const inline Quat Quat::zero{ 0.0f };

// Quat inline functions begin

// Quaternion multiplication
WAK_CPU_GPU constexpr inline Quat operator*(const Quat& a, const Quat& b)
{
    return Quat{
        a.w * b.x + b.w * a.x + a.y * b.z - b.y * a.z,
        a.w * b.y + b.w * a.y + a.z * b.x - b.z * a.x,
        a.w * b.z + b.w * a.z + a.x * b.y - b.x * a.y,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
    };
}

WAK_CPU_GPU constexpr inline Float Dot(const Quat& a, const Quat& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

WAK_CPU_GPU constexpr inline Quat operator+(const Quat& a, const Quat& b)
{
    return Quat(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

WAK_CPU_GPU constexpr inline Quat operator+(const Quat& a, Float b)
{
    return Quat(a.x + b, a.y + b, a.z + b, a.w + b);
}

WAK_CPU_GPU constexpr inline Quat operator-(const Quat& a, const Quat& b)
{
    return Quat(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

WAK_CPU_GPU constexpr inline Quat operator-(const Quat& a, Float b)
{
    return Quat(a.x - b, a.y - b, a.z - b, a.w - b);
}

WAK_CPU_GPU constexpr inline Quat operator*(const Quat& q, Float s)
{
    return Quat(q.x * s, q.y * s, q.z * s, q.w * s);
}

WAK_CPU_GPU constexpr inline Quat operator*(Float s, const Quat& q)
{
    return Quat(q.x * s, q.y * s, q.z * s, q.w * s);
}

// WAK_CPU_GPU constexpr inline Quat operator*(const Quat& a, const Quat& b)
// {
//     return Quat(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
// }

WAK_CPU_GPU constexpr inline Quat operator/(const Quat& q, Float s)
{
    return Quat(q.x / s, q.y / s, q.z / s, q.w / s);
}

WAK_CPU_GPU constexpr inline Quat operator/(Float s, const Quat& q)
{
    return Quat(s / q.x, s / q.y, s / q.z, s / q.w);
}

WAK_CPU_GPU constexpr inline Quat operator/(const Quat& a, const Quat& b)
{
    return Quat(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

WAK_CPU_GPU constexpr inline bool operator==(const Quat& a, const Quat& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

WAK_CPU_GPU constexpr inline bool operator!=(const Quat& a, const Quat& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

WAK_CPU_GPU constexpr inline Float Length2(const Quat& q)
{
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

WAK_CPU_GPU inline Float Length(const Quat& q)
{
    return std::sqrt(Length2(q));
}

WAK_CPU_GPU inline Quat Normalize(const Quat& q)
{
    Float inv_length = Float(1) / Length(q);
    return q * inv_length;
}

WAK_CPU_GPU inline Quat NormalizeSafe(const Quat& q)
{
    Float length = Length(q);
    if (length < std::numeric_limits<Float>::epsilon())
    {
        return Quat::zero;
    }

    Float inv_length = Float(1) / length;
    return q * inv_length;
}

// Compute angle between two quaternions
WAK_CPU_GPU inline Float Angle(const Quat& a, const Quat& b)
{
    return std::acos(Dot(a, b)) * 2;
}

// Quat inline functions end

} // namespace wak
