#pragma once

#include "Material.h"
#include "Ray.h"
#include "Traceable.h"
#include "Util.h"
#include <memory>
#include <optional>

class Sphere : public Traceable {
  public:
    Sphere(Point center, double radius, std::shared_ptr<Material> material)
        : ray_center(Ray{center, Vec{0}, 0}), radius(radius), mat(material) {}

    Sphere(const Point& center1, const Point& center2, double radius, std::shared_ptr<Material> material)
        : ray_center(Ray{center1, center2 - center1, 0}), radius(radius), mat(material) {}

  private:
    std::optional<Trace> trace(const Ray& ray, Interval t_interval) const override {
        Vec co = ray.origin() - ray_center.at(ray.time());
        double a = glm::dot(ray.direction(), ray.direction());
        double b = glm::dot(ray.direction(), co);
        double c = glm::dot(co, co) - radius * radius;

        double D = b * b - a * c;

        if (D < 0)
            return std::nullopt;

        double t = (-b - glm::sqrt(D)) / a;
        if (t_interval.surrounds(t) == false) {
            t = (-b + glm::sqrt(D)) / a;
            if (t_interval.surrounds(t) == false) {
                return std::nullopt;
            }
        }

        Point intersection = ray.at(t);
        return Trace{intersection, (intersection - ray_center.at(ray.time())) / radius, t, mat};
    }

  private:
    Ray ray_center;
    double radius;

    std::shared_ptr<Material> mat;
};