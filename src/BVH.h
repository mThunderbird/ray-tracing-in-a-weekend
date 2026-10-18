#pragma once

#include "AABB.h"
#include "Traceable.h"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>
class BVHNode : public Traceable {
  public:

    BVHNode(Scene scene) : BVHNode(scene.objects, 0, scene.objects.size()) {}

    BVHNode(std::vector<std::shared_ptr<Traceable>>& objects, size_t start, size_t end) {

        bbox = AABB::empty;
        for (size_t i = start; i < end; i++)
            bbox = AABB(bbox, objects[i]->bounding_box());

        int axis = bbox.longest_axis();

        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                        : box_z_compare;
        
        size_t object_span = end - start;
        if (object_span == 1) {
            left = objects[start];
            right = objects[start];
        }
        else if (object_span == 2) {
            left = objects[start];
            right = objects[start + 1];
        }
        else {
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);
            auto mid = start + object_span/2;
            left = std::make_shared<BVHNode>(objects, start, mid);
            right = std::make_shared<BVHNode>(objects, mid, end);
        }

    }

    bool trace(const Ray& ray, Interval t_interval, Trace& trace) const override {
        if (bbox.hit(ray, t_interval) == false)
            return false;

        bool hit_left = left->trace(ray, t_interval, trace);
        bool hit_right = right->trace(ray, Interval(t_interval.min, hit_left ? trace.t : t_interval.max), trace);

        return hit_left || hit_right;
    }

    AABB bounding_box() const override { return bbox; }

  private:
    std::shared_ptr<Traceable> left;
    std::shared_ptr<Traceable> right;
    AABB bbox;

    static bool box_compare(const std::shared_ptr<Traceable> a, const std::shared_ptr<Traceable> b, int axis_index) {
        auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
        auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
        return a_axis_interval.min < b_axis_interval.min;
    }

    static bool box_x_compare(const std::shared_ptr<Traceable> a, const std::shared_ptr<Traceable> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const std::shared_ptr<Traceable> a, const std::shared_ptr<Traceable> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const std::shared_ptr<Traceable> a, const std::shared_ptr<Traceable> b) {
        return box_compare(a, b, 2);
    }
};