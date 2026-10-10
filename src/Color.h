# pragma once

#include "Util.h"
#include "Interval.h"

#include <iostream>

inline void write_color(std::ostream& out, const Color& pixel_color) {

    static const Interval intensity(0.000, 0.999);
    int ir = int(256 * intensity.clamp(pixel_color.r));
    int ig = int(256 * intensity.clamp(pixel_color.g));
    int ib = int(256 * intensity.clamp(pixel_color.b));

    out << ir << ' ' << ig << ' ' << ib << '\n';
}
