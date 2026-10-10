#include "Util.h"
#include "Ray.h"
#include <optional>
#include <vector>
#include <memory>
#include "Interval.h"

struct Trace {
    Point p;
    Vec N;
    double t;
};

class Traceable {
  public:
    virtual ~Traceable() = default;

    virtual std::optional<Trace> trace(const Ray& ray, Interval t_interval) const = 0;
};

class TraceableList : public Traceable {
    public:
    std::vector<std::shared_ptr<Traceable>> objects;
    TraceableList() {}

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Traceable> object) { objects.push_back(object); }

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