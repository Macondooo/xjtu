
#pragma once

#include "../lib/mathlib.h"
#include "../util/hdr_image.h"

namespace Samplers {

struct Point {
    Point(Vec3 point) : point(point) {
    }

    Vec3 sample() const;
    Vec3 point;
};

using Direction = Point;

struct Rect {
    Rect(Vec2 size = Vec2(1.0f)) : size(size) {
    }
    Vec2 sample() const;
    Vec2 size;
};

struct Triangle {
    Triangle(Vec3 v0, Vec3 v1, Vec3 v2) : v0(v0), v1(v1), v2(v2) {
    }
    Vec3 sample() const;
    Vec3 v0, v1, v2;
};

namespace Hemisphere {

struct Uniform {
    Uniform() = default;
    Vec3 sample() const;
};

struct Cosine {
    Cosine() = default;
    Vec3 sample() const;
};
} // namespace Hemisphere

namespace Sphere {

struct Uniform {
    Uniform() = default;
    Vec3 sample() const;
    Hemisphere::Uniform hemi;
};

struct Image {
    Image(const HDR_Image& image);
    Vec3 sample() const;
    float pdf(Vec3 dir) const;

    size_t w = 0, h = 0;
    std::vector<float> _pdf, _cdf;
    float total = 0.0f;
};

} // namespace Sphere
} // namespace Samplers
