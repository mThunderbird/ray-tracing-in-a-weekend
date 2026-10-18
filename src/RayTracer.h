# pragma once

#include "BVH.h"
#include "Renderer.h"
#include "Sphere.h"
#include "Traceable.h"
#include <memory>

class RayTracer {
  public:
    RayTracer(int image_width, int image_height) :
        camera(double(image_width) / image_height),
        image_width(image_width),
        image_height(image_height) {}

    Image render(const Scene& scene) {

        Scene optimized_scene;
        optimized_scene.add(std::make_shared<BVHNode>(scene));

        Image image(image_width, image_height);
        renderer.render(camera, optimized_scene, image);
        return image;
    }

    void set_position(Point position) {
        camera.center = position;
        camera.invalidate();
    }

    void set_forward(Vec forward) {
        camera.forward = forward;
        camera.invalidate();
    }

    void set_up(Vec up) {
        camera.up = up;
        camera.invalidate();
    }

    void set_focus_distance(double focus_distance) {
        camera.f = focus_distance;
        camera.invalidate();
    }

    void set_vertical_field_of_view(double degrees) {
        camera.vfov = degrees_to_radians(degrees);
        camera.invalidate();
    }

    void set_defocus_angle(double degrees) {
        camera.defocus_angle = degrees_to_radians(degrees);
    }

    void set_samples_per_pixel(int samples) {
        renderer.samples_per_pixel = samples;
    }

    void set_max_depth(int depth) {
        renderer.max_depth = depth;
    }

  private:
    Camera camera;
    Renderer renderer;

    int image_width;
    int image_height;
};
