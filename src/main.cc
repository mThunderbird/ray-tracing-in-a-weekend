#include "Util.h"
#include "color.h"
#include "Camera.h"
#include <iostream>

double hit_sphere(const Point& center, double radius, const Ray& ray) {
    Vec co = ray.origin() - center;
    double a = glm::dot(ray.direction(), ray.direction());
    double b = 2 * glm::dot(ray.direction(), co);
    double c = glm::dot(co, co) - radius * radius; 
    
    double D = b * b - 4 * a * c;
    
    return (D >= 0) ? (- b - glm::sqrt(D)) / (2.0 * a) : - 1;
}

Color ray_color(const Ray& r) {

    Point center(0, 0, -1);
    double radius = 0.5;
    double t_hit = hit_sphere(center, radius, r);

    if(t_hit > 0.0) {
        Vec N = (r.at(t_hit) - center) / radius;
        return 0.5 * Color(N.x + 1, N.y + 1, N.z + 1);
    }

    Vec unit_dir = glm::normalize(r.direction());
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

    // Create the camera
    Camera camera(
        Point(0, 0, 0),
        Vec(0, 0, -1),
        Vec(0, 1, 0),
        1.0,
        viewport_width,
        viewport_height
    );


    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j = 0; j < image_height; j ++) {
        std::clog << "\rScanlines remaining: " << (image_height - 1) << " " << std::flush;

        for(int i = 0; i < image_width; i ++) {
            
            Ray r = camera.ray_through(double(i) / image_width, double(j)  / image_height);
            auto pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rRendering done                                    \n";
}
