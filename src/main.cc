#include "Camera.h"
#include "Color.h"
#include "Sphere.h"
#include "Util.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>

Color ray_color(const Ray& ray, const Traceable& world) {
    std::optional<Trace> trace = world.trace(ray, Interval{0, infinity});
    if(trace.has_value()) {
        return 0.5 * (trace->N + Color(1, 1, 1));
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
                pixel_color += ray_color(r, world);
            }
            pixel_color /= samples_per_pixel;
            
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rRendering done                                    \n";
}
