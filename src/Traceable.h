#include "Util.h"
#include "Ray.h"
#include <optional>

struct Trace {
    Point p;
    Vec N;
    double t;
};

class Traceable {
  public:
    virtual ~Traceable() = default;

    virtual std::optional<Trace> trace(const Ray& ray, double tmin, double tmax) const = 0;
};