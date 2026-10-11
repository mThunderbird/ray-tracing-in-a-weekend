# pragma once

#include "Util.h"
#include "Ray.h"
#include "glm/ext/vector_double2.hpp"
#include "glm/gtc/random.hpp"
#include "glm/trigonometric.hpp"
#include <cmath>
#include <vector>

class Camera {
  public:
        
    explicit Camera(
        const Point& center, 
        const Vec& forward, 
        const Vec& up, 
        double focal_length, 
        double vertical_field_of_view_degrees, 
        double defocus_angle_degrees,
        double aspect_ratio) :
        center(center),
        forward(glm::normalize(forward)),
        up(glm::normalize(up)),
        f(focal_length),
        vfov(degrees_to_radians(vertical_field_of_view_degrees)),
        defocus_angle(degrees_to_radians(defocus_angle_degrees)) {

          viewport_height = 2 * f * glm::tan(vfov / 2);
          viewport_width = aspect_ratio * viewport_height;

          prepare_for_frame();
        }
    
    Ray ray_through(double du, double dv) {

        if(is_prepared_for_frame == false) 
          prepare_for_frame();
      
        glm::dvec2 defocus_sample = defocus_angle > 0 ? glm::diskRand(f * std::tan(defocus_angle / 2)) : glm::dvec2(0);

        Point origin = center + defocus_sample.x * cached_left + defocus_sample.y * up;
        Point target = cached_top_left + cached_horziontal * du + cached_vertical * dv;

        return Ray(origin, glm::normalize(target - origin));
    }

  private:
    Point center;
    Vec forward;
    Vec up;

    Vec cached_left;
    Vec cached_horziontal;
    Vec cached_vertical;
    Vec cached_top_left;
    bool is_prepared_for_frame = false;

    double f;
    double vfov;
    double defocus_angle;

    double viewport_height;
    double viewport_width;

    void prepare_for_frame() {
      cached_left = glm::normalize(glm::cross(up, forward));

      Point top_left = center + forward * f + cached_left * viewport_width / 2.0 + up * viewport_height / 2.0;
      Point top_right = center + forward * f - cached_left * viewport_width / 2.0 + up * viewport_height / 2.0;
      Point bot_left = center + forward * f + cached_left * viewport_width / 2.0 - up * viewport_height / 2.0;

      cached_top_left = top_left;
      cached_horziontal = top_right - top_left;
      cached_vertical = bot_left - top_left;

      is_prepared_for_frame = true;
    }
};