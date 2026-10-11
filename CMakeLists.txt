cmake_minimum_required(VERSION 3.20)

project(RayTracingInAWeekend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glm)

add_library(raytracing INTERFACE)
target_include_directories(raytracing INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(raytracing INTERFACE glm::glm)
target_compile_features(raytracing INTERFACE cxx_std_23)

add_executable(raytracer src/main.cc)
target_link_libraries(raytracer PRIVATE raytracing)

if(MSVC)
    target_compile_options(raytracer PRIVATE /W4 /permissive-)
else()
    target_compile_options(raytracer PRIVATE -Wall -Wextra -Wpedantic)
endif()
