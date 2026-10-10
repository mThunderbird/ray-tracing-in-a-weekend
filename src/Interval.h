# pragma once

#include "Util.h"

struct Interval {

    double min, max;
    explicit Interval(double min, double max) :
        min(min),
        max(max) {}
    
    double size() const { return max - min; }
    bool contains(double x) const { return min <= x && x <= max; }
    bool surrounds(double x) const { return min < x && x < max; }
    double clamp(double x) const {
        if(x < min) return min;
        if(x > max) return max;
        return x;
    }

    static const Interval empty;
    static const Interval universe;
};

inline const Interval Interval::empty{+infinity, -infinity};
inline const Interval Interval::universe{-infinity, +infinity};
