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

double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

Color random_color() {
    return Color(random_double(), random_double(), random_double());
}

Color random_color(double min, double max) {
    return Color(random_double(min, max), random_double(min, max), random_double(min, max));
}

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
    
    // Image. 675 rows gives the cover's 1200x675 frame.
    double aspect_ratio = 16.0 / 9.0;
    int image_height = 675;
    int image_width = std::max(aspect_ratio * image_height, 1.0);

    int samples_per_pixel = 500;
    int max_depth = 50;
    // Camera setup
    Point lookfrom(13, 2, 3);
    Point lookat(0, 0, 0);
    Vec forward = lookat - lookfrom;
    Vec world_up(0, 1, 0);
    Vec up = world_up - glm::dot(world_up, glm::normalize(forward)) * glm::normalize(forward);

    Camera camera(
        lookfrom,
        forward,
        up,
        10.0,
        20,
        0.6,
        double(image_width) / image_height
    );

    // World
    TraceableList world;
    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            Point center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if (glm::length(center - Point(4, 0.2, 0)) > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    Color albedo = random_color() * random_color();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                } else if (choose_mat < 0.95) {
                    Color albedo = random_color(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                } else {
                    sphere_material = std::make_shared<Dielectric>(Color(1, 1, 1), 1.5);
                }

                world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
            }
        }
    }

    world.add(std::make_shared<Sphere>(Point(0, 1, 0), 1.0, std::make_shared<Dielectric>(Color(1, 1, 1), 1.5)));
    world.add(std::make_shared<Sphere>(Point(-4, 1, 0), 1.0, std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1))));
    world.add(std::make_shared<Sphere>(Point(4, 1, 0), 1.0, std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0)));


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
