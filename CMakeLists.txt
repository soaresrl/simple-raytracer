cmake_minimum_required ( VERSION 3.10 )

project(
	RayTracer
	VERSION 0.0.1
)

set(
	COMMON
	geometry/vec3.h
	geometry/color.h
	geometry/ray.h
)

set(
	OBJECTS
	objects/camera.h
	objects/directional_light.h
	objects/hittable.h
	objects/hittable_list.h
	objects/light.h
	objects/plane.h
	objects/point_light.h
	objects/scene.h
	objects/sphere.h
)

set(
	UTILITY
	utils/utility.h
)

add_executable(RayTracer raytracer.cpp ${COMMON} ${OBJECTS} ${UTILITY})

target_include_directories(RayTracer PRIVATE external/glm)
