#pragma once

#include "transform.h"

namespace wak
{

struct Ray
{
    Ray() = default;
    WAK_CPU_GPU Ray(const Point3& origin, const Vec3& direction);

    WAK_CPU_GPU Point3 At(Float t) const;

    Point3 o;
    Vec3 d;

    // todo: http://www.pbr-book.org/3ed-2018/Shapes/Managing_Rounding_Error.html
    static constexpr Float epsilon = Float(1e-4);
};

WAK_CPU_GPU inline Ray::Ray(const Point3& origin, const Vec3& direction)
    : o{ origin }
    , d{ direction }
{
}

WAK_CPU_GPU inline Point3 Ray::At(Float t) const
{
    return o + d * t;
}

WAK_CPU_GPU inline Ray Mul(const Transform& tf, const Ray& ray)
{
    return Ray(Mul(tf, ray.o), tf.q.Rotate(tf.s * ray.d));
}

WAK_CPU_GPU inline Ray MulT(const Transform& tf, const Ray& ray)
{
    return Ray(MulT(tf, ray.o), tf.q.RotateInv(ray.d / tf.s));
}

} // namespace wak