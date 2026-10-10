#include "Camera.h"
#include "Color.h"
#include "Sphere.h"
#include "Util.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>

#include <glm/gtc/random.hpp>


Vec random_on_hemisphere(const Vec& normal) {
    Vec random_unit = glm::sphericalRand(1.0);
    if(glm::dot(random_unit, normal) > 0.0)
        return random_unit;
    else
        return -random_unit;
}

Color ray_color(const Ray& ray, const Traceable& world, int depth) {
    if (depth <= 0)
        return Color(0, 0, 0);


    std::optional<Trace> trace = world.trace(ray, Interval{0.001, infinity});
    if(trace.has_value()) {
        Vec direction = trace->N + glm::sphericalRand(1.0);
        return 0.5 * ray_color(Ray(trace->p, direction), world, depth-1);
    }

    Vec unit_dir = glm::normalize(ray.direction());
    double a = 0.5 * (unit_dir.y + 1.0);
    return (1.0 - a)*Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}



int main() {
    
    // Image
    double aspect_ratio = 16.0 / 9.0;
    int image_height = 300;
    // Calculate image width and ensure it is atleast 1
    int image_width = std::max(aspect_ratio * image_height, 1.0);

    // Calculate viewport with true aspect ratio
    double viewport_height = 2.0;
    double viewport_width = viewport_height * double(image_width) / image_height;
    int samples_per_pixel = 100;
    int max_depth = 50;
    // Camera setup
    Camera camera(
        Point(0, 0, 0),
        Vec(0, 0, -1),
        Vec(0, 1, 0),
        1.0,
        viewport_width,
        viewport_height
    );

    // World 
    TraceableList world;
    world.add(std::make_shared<Sphere>(Point(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point(0, -100.5, -1), 100));


    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j = 0; j < image_height; j ++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

        for(int i = 0; i < image_width; i ++) {
            Color pixel_color(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; sample ++) {
                
                double du = double(i + random_double()) / image_width;
                double dv = double(j + random_double()) / image_height;

                Ray r = camera.ray_through(du, dv);
                pixel_color += ray_color(r, world, max_depth);
            }
            pixel_color /= samples_per_pixel;
            
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rRendering done                                    \n";
}
