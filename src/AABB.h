#pragma once
#include "Interval.h"
#include "Util.h"
#include "Ray.h"
#include <algorithm>

class AABB {
  public:
    AABB() : x(Interval::empty), y(Interval::empty), z(Interval::empty) {}
    AABB(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) {}

    explicit AABB(const Point& a, const Point& b) :
        x(std::min(a.x, b.x), std::max(a.x, b.x)),
        y(std::min(a.y, b.y), std::max(a.y, b.y)),
        z(std::min(a.z, b.z), std::max(a.z, b.z)) {}

    explicit AABB(const AABB& a, const AABB& b) :
        x(Interval(a.x, b.x)),
        y(Interval(a.y, b.y)),
        z(Interval(a.z, b.z)) {}

    const Interval& axis_interval(int n) const {
        if (n==1) return y;
        if (n==2) return z;
        return x;
    }

    bool hit(const Ray& incoming, Interval ray_t) const {
        
        for (int axis = 0; axis < 3; axis ++) {
            const Interval& ax = axis_interval(axis);
            double t0 = (ax.min - incoming.origin()[axis]) / incoming.direction()[axis];
            double t1 = (ax.max - incoming.origin()[axis]) / incoming.direction()[axis];
            
            double t_min = std::min(t0, t1);
            double t_max = std::max(t0, t1);

            ray_t.min = std::max(t_min, ray_t.min);
            ray_t.max = std::min(t_max, ray_t.max);

            if (ray_t.max <= ray_t.min) {
                return false;
            }
        }

        return  true;
    }

    int longest_axis() {
        if (x.size() > y.size())
            if (x.size() > z.size())
                return 0;
            else
                return 2;
        else if(y.size() > z.size())
            return  1;
        else
            return 2;
    }

    static const AABB empty;
    static const AABB universe;

  private:
    Interval x, y, z;
};

inline const AABB AABB::empty = AABB(Interval::empty, Interval::empty, Interval::empty);
inline const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);