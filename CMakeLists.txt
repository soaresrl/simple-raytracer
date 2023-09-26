cmake_minimum_required ( VERSION 3.10 )

project(
	RayTracer
	VERSION 0.0.1
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Gui OpenGL Widgets REQUIRED)

find_package(OpenGL REQUIRED)

set(
	COMMON
	geometry/vec3.h
	geometry/color.h
	geometry/ray.h
	geometry/orthonormalbasis.h
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
	objects/xy_plane.h
	objects/triangle.h
)

set(
	UTILITY
	utils/utility.h
	utils/raycolor.h
)

set(
	MATERIAL
	materials/dielectric.h
	materials/diffuse_light.h
	materials/material.h
	materials/metal.h
	materials/lambertian.h
)

set(
	SAMPLING
	sampling/pdf.h
	sampling/cosine_pdf.h
)

set (
	OCTREE
	octree/voxel.h
	octree/octree.h
	octree/octree.cpp
	octree/primitives/block.h
	octree/primitives/cone.h
	octree/primitives/cylinder.h
	octree/primitives/solid.h
	octree/primitives/sphere.h
	octree/boolean_operations/boolean_operations.h
	octree/boolean_operations/boolean_operations.cpp
)

set (
	UI
	ui/mainwindow.h
	ui/mainwindow.cpp
	ui/mainwindow.ui
	ui/canvas.h
	ui/canvas.cpp
	ui/camera/flycamera.h
	ui/camera/flycamera.cpp
	ui/tree_item.h
	ui/tree_model.h
	ui/tree_model.cpp
)

#add_executable(RayTracer raytracer.cpp ${COMMON} ${OBJECTS} ${UTILITY} ${SAMPLING} ${OCTREE})
add_executable(RayTracer main.cpp ${COMMON} ${OBJECTS} ${UTILITY} ${SAMPLING} ${OCTREE} ${UI})

#add_executable(Voxelizer voxelizer.cpp ${COMMON} ${OBJECTS} ${UTILITY} ${SAMPLING} ${OCTREE})

target_link_libraries(RayTracer PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL opengl32)

add_custom_command(TARGET RayTracer POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/shaders" "${CMAKE_BINARY_DIR}/shaders")
#target_include_directories(RayTracer PRIVATE external/glm)

#add_custom_command(TARGET RayTracer POST_BUILD 
#                   COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets")