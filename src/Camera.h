# pragma once

#include "Util.h"
#include "Ray.h"
#include <vector>

class Camera {
  public:
        
    explicit Camera(
        const Point& center, 
        const Vec& forward, 
        const Vec& up, 
        double focal_length, 
        double viewport_width, 
        double viewport_height) :
        center(center),
        forward(glm::normalize(forward)),
        up(glm::normalize(up)),
        f(focal_length),
        viewport_width(viewport_width),
        viewport_height(viewport_height) {}
    
    Ray ray_through(double du, double dv) const {
        
        std::vector<Ray> rays;

        Vec left = glm::normalize(glm::cross(up, forward));

        Point top_left = forward * f + left * viewport_width / 2.0 + up * viewport_height / 2.0;
        Point top_right = forward * f - left * viewport_width / 2.0 + up * viewport_height / 2.0;
        Point bot_left = forward * f + left * viewport_width / 2.0 - up * viewport_height / 2.0;

        Vec horizontal = top_right - top_left;
        Vec vertical = bot_left - top_left;

        return Ray(
            center, 
            glm::normalize( top_left + horizontal * du + vertical * dv - center)
        );
    }

  private:
    Point center;
    Vec forward;
    Vec up;
    double f;
    double viewport_width;
    double viewport_height;
};