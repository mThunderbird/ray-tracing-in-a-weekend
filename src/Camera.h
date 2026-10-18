#pragma once

#include "Util.h"
#include "Ray.h"
#include "glm/ext/vector_double2.hpp"
#include "glm/gtc/random.hpp"
#include "glm/trigonometric.hpp"
#include <cmath>

class Renderer;
class RayTracer;

class Camera {
    friend class RayTracer;
    friend class Renderer;

  private:
    explicit Camera(double aspect_ratio) : aspect_ratio(aspect_ratio) {}

    Point center = Point(0, 0, 0);
    Vec forward = Vec(0, 0, -1);
    Vec up = Vec(0, 1, 0);

    double f = 1.0;
    double vfov = degrees_to_radians(90.0);
    double defocus_angle = degrees_to_radians(0.0);

    void invalidate() { is_prepared_for_frame = false; }

    Ray ray_through(double du, double dv) {

        if (is_prepared_for_frame == false)
            prepare_for_frame();

        glm::dvec2 defocus_sample = defocus_angle > 0 ? glm::diskRand(f * std::tan(defocus_angle / 2)) : glm::dvec2(0);

        Point origin = center + defocus_sample.x * cached_left + defocus_sample.y * cached_up;
        Point target = cached_top_left + cached_horziontal * du + cached_vertical * dv;

        return Ray(origin, glm::normalize(target - origin), random_double());
    }

  private:
    double aspect_ratio = 1.0;

    Vec cached_left;
    Vec cached_up;
    Vec cached_horziontal;
    Vec cached_vertical;
    Vec cached_top_left;
    bool is_prepared_for_frame = false;

    double viewport_height = 0;
    double viewport_width = 0;

    void prepare_for_frame() {
        viewport_height = 2 * f * glm::tan(vfov / 2);
        viewport_width = aspect_ratio * viewport_height;

        Vec unit_forward = glm::normalize(forward);
        cached_up = glm::normalize(up);
        cached_left = glm::normalize(glm::cross(cached_up, unit_forward));

        Point top_left =
            center + unit_forward * f + cached_left * viewport_width / 2.0 + cached_up * viewport_height / 2.0;
        Point top_right =
            center + unit_forward * f - cached_left * viewport_width / 2.0 + cached_up * viewport_height / 2.0;
        Point bot_left =
            center + unit_forward * f + cached_left * viewport_width / 2.0 - cached_up * viewport_height / 2.0;

        cached_top_left = top_left;
        cached_horziontal = top_right - top_left;
        cached_vertical = bot_left - top_left;

        is_prepared_for_frame = true;
    }
};