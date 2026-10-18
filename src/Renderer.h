# pragma once

#include "Camera.h"
#include "Image.h"
#include "Material.h"
#include "Traceable.h"

#include <iostream>

class RayTracer;

class Renderer {
    friend class RayTracer;

  private:
    Renderer() = default;

    int samples_per_pixel = 20;
    int max_depth = 10;

    void render(Camera& camera, const Traceable& scene, Image& image) const {
        for (int j = 0; j < image.height; j++) {
            std::clog << "\rScanlines remaining: " << (image.height - j) << ' ' << std::flush;

            for (int i = 0; i < image.width; i++) {
                Color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    double du = double(i + random_double()) / image.width;
                    double dv = double(j + random_double()) / image.height;

                    Ray ray = camera.ray_through(du, dv);
                    pixel_color += ray_color(ray, scene, max_depth);
                }
                pixel_color /= samples_per_pixel;
                image.at(i, j) = pixel_color;
            }
        }
        std::clog << "\rRendering done                                    \n";
    }

    Color ray_color(const Ray& ray, const Traceable& scene, int depth) const {
        if (depth <= 0)
            return Color(0, 0, 0);

        Trace trace;
        if (scene.trace(ray, Interval{0.001, infinity}, trace)) {
            std::optional<Scatter> scatter = trace.mat->scatter(ray, trace);
            if (scatter)
                return scatter->attenuation * ray_color(scatter->ray, scene, depth - 1);
            return Color(0, 0, 0);
        }

        Vec unit_dir = glm::normalize(ray.direction());
        double a = 0.5 * (unit_dir.y + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }
};
