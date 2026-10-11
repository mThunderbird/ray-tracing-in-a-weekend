# pragma  once 

#include "Interval.h"
#include "Ray.h"
#include "Trace.h"

#include <memory>
#include <optional>
#include <vector>

class Renderer;
class Scene;

class Traceable {
    friend class Renderer;
    friend class Scene;

  public:
    virtual ~Traceable() = default;

  private:
    virtual std::optional<Trace> trace(const Ray& ray, Interval t_interval) const = 0;
};

class Scene : public Traceable {
  public:
    void clear() { objects.clear(); }
    void add(std::shared_ptr<Traceable> object) { objects.push_back(object); }

  private:
    std::vector<std::shared_ptr<Traceable>> objects;

    std::optional<Trace> trace(const Ray& ray, Interval t_interval) const override {
     
        std::optional<Trace> trace = std::nullopt;

        for(const auto& object : objects) {
            std::optional<Trace> tmp = object->trace(ray, t_interval);
            if (tmp.has_value()) {
                trace = tmp;
                t_interval.max = tmp->t;        
            }
        }

        return trace;
    }
};