# pragma once

#include "Util.h"
#include "Interval.h"

#include <cmath>
#include <iostream>

inline double linear_to_gamma(double linear_component) {
    return linear_component > 0 ? std::sqrt(linear_component) : 0;
}

inline void write_color(std::ostream& out, const Color& pixel_color) {

    auto r = linear_to_gamma(pixel_color.r);
    auto g = linear_to_gamma(pixel_color.g);
    auto b = linear_to_gamma(pixel_color.b);


    static const Interval intensity(0.000, 0.999);
    int ir = int(256 * intensity.clamp(r));
    int ig = int(256 * intensity.clamp(g));
    int ib = int(256 * intensity.clamp(b));

    out << ir << ' ' << ig << ' ' << ib << '\n';
}
