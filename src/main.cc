#include "Camera.h"
#include "Color.h"
#include "Material.h"
#include "Sphere.h"
#include "Util.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>

#include <glm/gtc/random.hpp>

Color ray_color(const Ray& ray, const Traceable& world, int depth) {
    if (depth <= 0)
        return Color(0, 0, 0);


    std::optional<Trace> trace = world.trace(ray, Interval{0.001, infinity});
    if(trace) {
        
        std::optional<Scatter> scatter = trace->mat->scatter(ray, trace.value());
        if(scatter) {
            return scatter->attenuation * ray_color(scatter->ray, world, depth-1);
        }
        return Color(0, 0, 0);
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
    int samples_per_pixel = 100;
    int max_depth = 50;
    // Camera setup
    Point lookfrom(-2, 2, 1);
    Point lookat(0, 0, -1);
    Vec forward = lookat - lookfrom;
    Vec world_up(0, 1, 0);
    Vec up = world_up - glm::dot(world_up, glm::normalize(forward)) * glm::normalize(forward);

    Camera camera(
        lookfrom,
        forward,
        up,
        glm::length(forward),
        20,
        10,
        double(image_width) / image_height
    );

    // World 
    TraceableList world;
    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Dielectric>(Color(0.4, 0.4, 0.2), 1.5);
    auto material_bubble = std::make_shared<Dielectric>(Color(1, 0.3, 0.3), 1.00 / 1.50);
    auto material_right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    world.add(std::make_shared<Sphere>(Point( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Point( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(std::make_shared<Sphere>(Point(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<Sphere>(Point(-1.0, 0.0, -1.0), 0.3, material_bubble));
    world.add(std::make_shared<Sphere>(Point( 1.0,    0.0, -1.0),   0.5, material_right));


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
