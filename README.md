# Ray Tracing in a Weekend

A C++ ray tracer, written while following [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Peter Shirley, Trevor David Black, and Steve Hollasch.

![Cover scene: a field of small spheres, a glass ball, and a chrome ball](images/cover.png)

## Features

- Spheres
- Matte, fuzzy metal, and tinted glass
- Hollow glass, from a bubble with the inverse refraction index
- A camera you can place and aim, with field of view and defocus blur
- Antialiasing from many rays per pixel

![Still life: blue matte, hollow glass, and gold metal on a yellow ground](images/still-life.png)

![Chrome between a red glass sphere and a hollow blue one](images/reflections.png)

## Run

```
python render.py release
```

CMake fetches GLM, builds the tracer, and writes `image.png`. The scene is in `src/main.cc`.
