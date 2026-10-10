# pragma once

#include <limits>
#include <numbers>

#include <glm/glm.hpp>

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = std::numbers::pi;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

using Color = glm::dvec3;
using Point = glm::dvec3;
using Vec = glm::dvec3;
