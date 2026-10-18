#pragma once

#include "Util.h"
#include <algorithm>

struct Interval {

    double min, max;
    explicit Interval(double min, double max) : min(min), max(max) {}

    explicit Interval(const Interval& a, const Interval& b)
        : min(std::min(a.min, b.min)), max(std::max(a.max, b.max)) {}

    double size() const { return max - min; }
    bool contains(double x) const { return min <= x && x <= max; }
    bool surrounds(double x) const { return min < x && x < max; }
    double clamp(double x) const {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }
    Interval pad(double delta) const { return Interval(min - delta, max + delta); }

    static const Interval empty;
    static const Interval universe;
};

inline const Interval Interval::empty{+infinity, -infinity};
inline const Interval Interval::universe{-infinity, +infinity};
