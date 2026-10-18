#pragma once

#include "AABB.h"
#include "Interval.h"
#include "Ray.h"
#include "Trace.h"

#include <memory>
#include <vector>

class Renderer;
class Scene;
class BVHNode;

class Traceable {
    friend class Renderer;
    friend class Scene;
    friend class BVHNode;

  public:
    virtual ~Traceable() = default;

  private:
    virtual bool trace(const Ray& ray, Interval t_interval, Trace& trace) const = 0;
    virtual AABB bounding_box() const = 0;
};

class Scene : public Traceable {
  friend class BVHNode;
  
  public:
    void clear() { objects.clear(); }
    void add(std::shared_ptr<Traceable> object) {
        objects.push_back(object);
        bbox = AABB(bbox, object->bounding_box());
    }

    
    private:
    std::vector<std::shared_ptr<Traceable>> objects;
    AABB bbox;

    bool trace(const Ray& ray, Interval t_interval, Trace& trace) const override {
        bool hit_anything = false;

        for (const auto& object : objects) {
            if (object->trace(ray, t_interval, trace)) {
                hit_anything = true;
                t_interval.max = trace.t;
            }
        }

        return hit_anything;
    }

    AABB bounding_box() const override { return bbox; }
};