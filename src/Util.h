# pragma once

#include "glm/gtc/epsilon.hpp"
#include "glm/vector_relational.hpp"
#include <cmath>
#include <limits>
#include <numbers>

#include <glm/glm.hpp>
#include <random>

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = std::numbers::pi;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> U(0.0, 1.0);
    static std::mt19937 generator;

    return U(generator);
}

using Color = glm::dvec3;
using Point = glm::dvec3;
using Vec = glm::dvec3;

inline bool near_zero(const Vec& v) {
    return glm::all(glm::epsilonEqual(v, Vec(0.0), 1e-8));
}
