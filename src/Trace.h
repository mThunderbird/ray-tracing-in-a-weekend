# pragma once

#include "Util.h"

#include <memory>

class Material;

struct Trace {
    Point p;
    Vec N;
    double t;
    std::shared_ptr<Material> mat;
};
