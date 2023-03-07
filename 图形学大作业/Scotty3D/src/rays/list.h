
#pragma once

#include "../lib/mathlib.h"
#include "../util/rand.h"
#include "trace.h"

namespace PT {

template<typename Primitive> class List {
public:
    List() {
    }
    List(std::vector<Primitive>&& primitives) : prims(std::move(primitives)) {
    }

    BBox bbox() const {
        BBox ret;
        for(const auto& p : prims) {
            ret.enclose(p.bbox());
        }
        return ret;
    }

    Trace hit(const Ray& ray) const {
        Trace ret;
        for(const auto& p : prims) {
            Trace test = p.hit(ray);
            ret = Trace::min(ret, test);
        }
        return ret;
    }

    void append(Primitive&& prim) {
        prims.push_back(std::move(prim));
    }

    List<Primitive> copy() const {
        std::vector<Primitive> prim_copy = prims;
        return List<Primitive>(std::move(prim_copy));
    }

    Vec3 sample(Vec3 from) const {
        if(prims.empty()) return {};
        int n = RNG::integer(0, (int)prims.size());
        return prims[n].sample(from);
    }

    float pdf(Ray ray, const Mat4& T = Mat4::I, const Mat4& iT = Mat4::I) const {
        if(prims.empty()) return 0.0f;
        float ret = 0.0f;
        for(auto& prim : prims) ret += prim.pdf(ray, T, iT);
        return ret / prims.size();
    }

    void clear() {
        prims.clear();
    }

    bool empty() const {
        return prims.empty();
    }

private:
    std::vector<Primitive> prims;
};

} // namespace PT
