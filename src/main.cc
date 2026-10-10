#include "Util.h"
#include "color.h"
#include "Camera.h"
#include <iostream>
#include <optional>
#include "Sphere.h"

Color ray_color(const Ray& r) {

    Sphere sphere(Point{0, 0, -1}, 0.5);
    std::optional<Trace> trace = sphere.trace(r, 0, 100);

    if(trace.has_value()) {
        Vec N = trace->N;
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
