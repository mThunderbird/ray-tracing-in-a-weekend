# pragma once

#include "Util.h"
#include "Ray.h"
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
        double aspect_ratio) :
        center(center),
        forward(glm::normalize(forward)),
        up(glm::normalize(up)),
        f(focal_length),
        vfov(degrees_to_radians(vertical_field_of_view_degrees)),
        aspect(aspect_ratio) {

          viewport_height = 2 * f * glm::tan(vfov / 2);
          viewport_width = aspect_ratio * viewport_height;

          prepare_for_frame();
        }
    
    Ray ray_through(double du, double dv) {

        if(is_prepared_for_frame == false) 
          prepare_for_frame();

        return Ray(
            center, 
            glm::normalize(cached_top_left + cached_horziontal * du + cached_vertical * dv - center)
        );
    }

  private:
    Point center;
    Vec forward;
    Vec up;

    Vec cached_horziontal;
    Vec cached_vertical;
    Vec cached_top_left;
    bool is_prepared_for_frame = false;

    double f;
    double vfov;
    double aspect;

    double viewport_height;
    double viewport_width;

    void prepare_for_frame() {
      Vec left = glm::normalize(glm::cross(up, forward));

      Point top_left = center + forward * f + left * viewport_width / 2.0 + up * viewport_height / 2.0;
      Point top_right = center + forward * f - left * viewport_width / 2.0 + up * viewport_height / 2.0;
      Point bot_left = center + forward * f + left * viewport_width / 2.0 - up * viewport_height / 2.0;

      cached_top_left = top_left;
      cached_horziontal = top_right - top_left;
      cached_vertical = bot_left - top_left;

      is_prepared_for_frame = true;
    }
};