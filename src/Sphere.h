#include "Ray.h"
#include "Traceable.h"
#include "Util.h"
#include <optional>

class Sphere : public Traceable {
  public:
    Sphere(Point center, double radius) :
        center(center),
        radius(radius) {}

    std::optional<Trace> trace(const Ray& ray, double tmin, double tmax) const override {
        Vec co = ray.origin() - center;
        double a = glm::dot(ray.direction(), ray.direction());
        double b = glm::dot(ray.direction(), co);
        double c = glm::dot(co, co) - radius * radius; 
        
        double D = b * b - a * c;
        
        if(D < 0) return std::nullopt;

        double t = (- b - glm::sqrt(D)) / a;
        if(t <= tmin || t >= tmax) {
            t = (- b + glm::sqrt(D)) / a;
            if(t <= tmin || t >= tmax) {
                return std::nullopt;
            }
        }

        Point intersection = ray.at(t);
        return Trace{intersection, (intersection - center) / radius, t};
    }

  private:
    Point center;
    double radius;
};