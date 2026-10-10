# pragma once

#include "Ray.h"
#include "Traceable.h"
#include "Util.h"
#include "glm/geometric.hpp"
#include "glm/gtc/random.hpp"
#include <optional>

struct Scatter {
  Ray ray;
  Color attenuation;
};

class Material {
  public:

    virtual ~Material() = default;

    virtual std::optional<Scatter> scatter([[maybe_unused]] const Ray& in, [[maybe_unused]] const Trace& trace) const {
        return std::nullopt;
    }
};


class Diffuse : public Material {
  public:
    Diffuse(const Color& albedo) :
      albedo(albedo) {}
    
    std::optional<Scatter> scatter([[maybe_unused]] const Ray& in, const Trace& trace) const override {
      
      Vec scatter_direction = random_on_hemisphere(trace.N);

      return Scatter(
        Ray(trace.p, scatter_direction),
        albedo);
    }

  private:

  Vec random_on_hemisphere(const Vec& normal) const {
    Vec random_unit = glm::sphericalRand(1.0);
    if(glm::dot(random_unit, normal) > 0.0)
      return random_unit;
    else
      return -random_unit;
  }

    Color albedo;
};

class Lambertian : public Material {
  public:
    Lambertian(const Color& albedo) :
      albedo(albedo) {}
    
      std::optional<Scatter> scatter([[maybe_unused]] const Ray& in, const Trace& trace) const override {
      
      Vec scatter_direction = trace.N + glm::sphericalRand(1.0);
      if (near_zero(scatter_direction))
        scatter_direction = trace.N;
      
      return Scatter(
        Ray(trace.p, scatter_direction),
        albedo);
    }

  private:
    Color albedo;
};

class Metal : public Material {
  public:
    Metal(const Color& albedo, double fuzz) :
      albedo(albedo),
      fuzz(std::min(fuzz, 1.0)) {}
    
    std::optional<Scatter> scatter(const Ray& in, const Trace& trace) const override {
      
      Vec reflected_dir = glm::normalize(glm::reflect(in.direction(), trace.N));
      Vec scatter_direction = reflected_dir + (fuzz * glm::sphericalRand(1.0));
      
      if (glm::dot(scatter_direction, trace.N) <= 0)
        return std::nullopt;

      return Scatter(
        Ray(trace.p, scatter_direction),
        albedo);
    }

  private:
    Color albedo;
    double fuzz;
};