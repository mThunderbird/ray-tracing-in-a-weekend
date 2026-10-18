#pragma once

#include "Util.h"

class Ray {
  public:
    explicit Ray(const Point& origin, const Vec& direction, double time) : o(origin), d(direction), tm(time) {}

    const Point& origin() const { return o; }
    const Vec& direction() const { return d; }
    double time() const { return tm; }

    Point at(double t) const { return o + t * d; }

  private:
    Point o;
    Vec d;
    double tm;
};