# pragma once

#include "Interval.h"
#include "Util.h"

#include <cmath>
#include <iostream>
#include <vector>

class Image {
  public:
    Image(int width, int height) :
        width(width),
        height(height),
        pixels(width * height, Color(0)) {}

    Color& at(int i, int j) { return pixels[j * width + i]; }
    const Color& at(int i, int j) const { return pixels[j * width + i]; }

    void write_ppm(std::ostream& out) const {
        out << "P3\n" << width << ' ' << height << "\n255\n";
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++)
                write_color(out, at(i, j));
        }
    }

    int width;
    int height;

  private:
    std::vector<Color> pixels;

    static double linear_to_gamma(double linear_component) {
        return linear_component > 0 ? std::sqrt(linear_component) : 0;
    }

    static void write_color(std::ostream& out, const Color& pixel_color) {
        double r = linear_to_gamma(pixel_color.r);
        double g = linear_to_gamma(pixel_color.g);
        double b = linear_to_gamma(pixel_color.b);

        static const Interval intensity(0.000, 0.999);
        int ir = int(256 * intensity.clamp(r));
        int ig = int(256 * intensity.clamp(g));
        int ib = int(256 * intensity.clamp(b));

        out << ir << ' ' << ig << ' ' << ib << '\n';
    }
};
