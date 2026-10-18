#include "RayTracer.h"

#include <algorithm>
#include <iostream>
#include <memory>

double random_double(double min, double max) { return min + (max - min) * random_double(); }

Color random_color() { return Color(random_double(), random_double(), random_double()); }

Color random_color(double min, double max) {
    return Color(random_double(min, max), random_double(min, max), random_double(min, max));
}

void add_random_spheres(Scene& scene) {
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            Point center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if (glm::length(center - Point(4, 0.2, 0)) > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    Color albedo = random_color() * random_color();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec(0, random_double(0, 0.5), 0);
                    scene.add(std::make_shared<Sphere>(center, center2, 0.2, sphere_material));
                    continue;
                } else if (choose_mat < 0.95) {
                    Color albedo = random_color(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                } else {
                    sphere_material = std::make_shared<Dielectric>(Color(1, 1, 1), 1.5);
                }

                scene.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
            }
        }
    }
}

void add_bouncing_spheres(Scene& scene) {
    auto ground = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    scene.add(std::make_shared<Sphere>(Point(0, -1000, 0), 1000, ground));
    add_random_spheres(scene);
    scene.add(std::make_shared<Sphere>(Point(0, 1, 0), 1.0, std::make_shared<Dielectric>(Color(1, 1, 1), 1.5)));
    scene.add(std::make_shared<Sphere>(Point(-4, 1, 0), 1.0, std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1))));
    scene.add(std::make_shared<Sphere>(Point(4, 1, 0), 1.0, std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0)));
}

void add_simple_scene(Scene& scene) {
    auto ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto left = std::make_shared<Dielectric>(Color(0.4, 0.4, 0.2), 1.5);
    auto bubble = std::make_shared<Dielectric>(Color(1, 0.3, 0.3), 1.00 / 1.50);
    auto right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    scene.add(std::make_shared<Sphere>(Point(0.0, -100.5, -1.0), 100.0, ground));
    scene.add(std::make_shared<Sphere>(Point(0.0, 0.0, -1.2), 0.5, center));
    scene.add(std::make_shared<Sphere>(Point(-1.0, 0.0, -1.0), 0.5, left));
    scene.add(std::make_shared<Sphere>(Point(-1.0, 0.0, -1.0), 0.3, bubble));
    scene.add(std::make_shared<Sphere>(Point(1.0, 0.0, -1.0), 0.5, right));
}

int main() {
    double aspect_ratio = 16.0 / 9.0;
    int image_height = 225;
    int image_width = std::max(aspect_ratio * image_height, 1.0);

    Point lookfrom(13, 2, 3);
    Point lookat(0, 0, 0);
    Vec forward = lookat - lookfrom;
    Vec world_up(0, 1, 0);
    Vec up = world_up - glm::dot(world_up, glm::normalize(forward)) * glm::normalize(forward);

    RayTracer tracer(image_width, image_height);
    tracer.set_position(lookfrom);
    tracer.set_forward(forward);
    tracer.set_up(up);

    tracer.set_focus_distance(10.0);
    tracer.set_defocus_angle(0.6);
    tracer.set_vertical_field_of_view(20);

    tracer.set_samples_per_pixel(100);
    tracer.set_max_depth(50);

    Scene scene;
    add_bouncing_spheres(scene);

    Image image = tracer.render(scene);
    image.write_ppm(std::cout);
}
