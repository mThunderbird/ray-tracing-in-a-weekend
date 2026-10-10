# pragma once

#include "Util.h"

class Ray {
  public:

  explicit Ray(const Point& origin, const Vec& direction) :
        o(origin),
        d(direction)
        {}
    
    const Point& origin() const { return o; }
    const Vec& direction() const { return d; }

    Point at(double t) const {
        return o + t * d;
    }

  private:

    Point o;
    Vec d;
};