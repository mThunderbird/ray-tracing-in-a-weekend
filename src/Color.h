# pragma once

#include "Util.h"

#include <iostream>

inline void write_color(std::ostream& out, const Color& pixel_color) {

    int ir = int(255.999 * pixel_color.r);
    int ig = int(255.999 * pixel_color.g);
    int ib = int(255.999 * pixel_color.b);

    out << ir << ' ' << ig << ' ' << ib << '\n';
}
