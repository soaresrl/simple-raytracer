#include <iostream>
#include <string>
#include <fstream>

#include "utils/utility.h"
#include "utils/raycolor.h"

#include "io/stl.h"

#include "geometry/color.h"
#include "objects/hittable_list.h"
#include "objects/sphere.h"
#include "objects/scene.h"
#include "objects/camera.h"
#include "objects/point_light.h"
#include "objects/directional_light.h"
#include "objects/light.h"
#include "objects/plane.h"
#include "objects/xy_plane.h"
#include "objects/flip_face.h"
#include "objects/triangle.h"

#include "materials/diffuse_light.h"
#include "materials/dielectric.h"
#include "materials/lambertian.h"
#include "materials/metal.h"

#include "octree/primitives/sphere.h"
#include "octree/primitives/block.h"
#include "octree/primitives/cylinder.h"
#include "octree/primitives/cone.h"
#include "octree/octree.h"
#include "octree/boolean_operations/boolean_operations.h"

scene cornell_box() {
    scene objects;

    auto red = make_shared<Lambertian>(color(.65, .05, .05));
    auto white = make_shared<Lambertian>(color(.73, .73, .73));
    auto green = make_shared<Lambertian>(color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(color(15, 15, 15));

    //objects.add_object(make_shared<Triangle>(point3{ -3.0, 0.0, 0.0 }, point3{ 0.0, 5.0, 0.0 }, point3{ 3.0, 0.0, 0.0 }, red));
    objects.add_object(make_shared<YZ_Plane>(point3{ -5, 0.0, 0.0 }, 10, 10, green));
    objects.add_object(make_shared<YZ_Plane>(point3{ 5, 0.0, 0.0 }, 10, 10, red));
    objects.add_object(make_shared<FlipFace>(make_shared<XZ_Plane>(point3{ 0.0, 4.995, 0.0 }, 5, 5, light)));
    objects.add_object(make_shared<FlipFace>(make_shared<YZ_Plane>(point3{ 0.0, 0.0, 0.0 }, 5, 5, light)));
    objects.add_object(make_shared<XZ_Plane>(point3{ 0.0, 5, 0.0 }, 10, 10, white));
    objects.add_object(make_shared<XZ_Plane>(point3{ 0.0, -5, 0.0 }, 10, 10, white));
    objects.add_object(make_shared<XY_Plane>(point3{ 0.0, 0.0, -5 }, 10, 10, white));

    auto dielectric = make_shared<Dielectric>(1.75);
    auto metal = make_shared<Metal>(color{ 1.0, 1.0, 1.0 }, 0.5);
    //objects.add_object(make_shared<sphere>(point3{ 0.0, -3.5, 0.0 }, 1.5, dielectric));
    //objects.add_object(make_shared<sphere>(point3{ -3.0, -3.5, 0.0 }, 1.5, red));

    //Sphere sphere1{ point3{0.0f, -3.5f, 0.0f}, 1.5 };
    Block block1{ point3{0.0, -3.5f, 0.0f}, 3.0 };
    Sphere sphere2{ point3{1.5f, -2.0f, 0.0}, 1.5 };
    // Block box{ point3{0.0, -3.5f, 0.0f}, 3.0 };
    // Cylinder cylinder{ point3{0.0, -3.5f, 0.0}, 1.5, 3.0 };
    // Cone cone{ point3{0.0, -3.5f, 0.0f}, 1.5, 3.0 };

    block1.ComputeBoundingBox();
    sphere2.ComputeBoundingBox();

    point3 bb_min;
    point3 bb_max;

    bb_min.setX(block1.min.x() < sphere2.min.x() ? block1.min.x() : sphere2.min.x());
    bb_min.setY(block1.min.y() < sphere2.min.y() ? block1.min.y() : sphere2.min.y());
    bb_min.setZ(block1.min.z() < sphere2.min.z() ? block1.min.z() : sphere2.min.z());

    bb_max.setX(block1.max.x() > sphere2.max.x() ? block1.max.x() : sphere2.max.x());
    bb_max.setY(block1.max.y() > sphere2.max.y() ? block1.max.y() : sphere2.max.y());
    bb_max.setZ(block1.max.z() > sphere2.max.z() ? block1.max.z() : sphere2.max.z());

    double size = max(max((bb_max.x() - bb_min.x()), bb_max.y() - bb_min.y()), (bb_max.z() - bb_min.z()));

    point3 center = (bb_max + bb_min) / 2.0f;

    shared_ptr<Voxel> octree1 = make_shared<Voxel>( size, center, red);
    shared_ptr<Voxel> octree2 = make_shared<Voxel>( size, center, red );

    BuildOctree(&block1, octree1, 5);
    BuildOctree(&sphere2, octree2, 5);

    shared_ptr<Voxel> intersection_octree = Difference(octree1, octree2);
    std::ofstream out_octree("17_09_TREE.txt");

    WriteFromTraverse(intersection_octree, out_octree);

    objects.add_object(intersection_octree);
    //objects.add_object(octree1);
    //objects.add_object(octree2);

    return objects;
}

int main() {
    // Image Properties
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_bounce = 5;

    float refractive = 2.25;

    //float total_frames = 1;

    //int i = 1;
    
    //while (refractive <= 2.5)
    //{
    std::ofstream outdata("19_09_Inter.ppm"/* + std::to_string(refractive) + ".ppm"*/);

        // world 
        scene world = cornell_box();

        //scene world;

            
        //auto objs = FromOBJ("./assets/ob1.obj");

        /*for (auto& obj : objs)
        {
            world.add_object(obj);
        }*/

        auto white = make_shared<Lambertian>(color(.73, .73, .73));

        /*world.add_object(make_shared<XZ_Plane>(point3{ 0.0, -18, 0.0 }, 36, 36, white));
        world.add_object(make_shared<XZ_Plane>(point3{ 0.0, 18, 0.0 }, 36, 36, white));
        world.add_object(make_shared<YZ_Plane>(point3{ -18, 0.0, 0.0 }, 36, 36, white));
        world.add_object(make_shared<YZ_Plane>(point3{ 18, 0.0, 0.0 }, 36, 36, white));
        world.add_object(make_shared<XY_Plane>(point3{ 0.0, 0.0, -10.0 }, 36, 36, white));*/

        //auto light = make_shared<DiffuseLight>(color(15, 15, 15));
        //world.add_object(make_shared<FlipFace>(make_shared<XZ_Plane>(point3{ 0.0, 17.995, 0.0 }, 15, 15, light)));

        auto lights = make_shared<hittable_list>();
        auto light_importance = make_shared<DiffuseLight>(color(15, 15, 15));

        lights->add(make_shared<XZ_Plane>(point3{ 0.0, 4.995, 0.0 }, 5, 5, light_importance));
        lights->add(make_shared<YZ_Plane>(point3{ 0.0, 0.0, 0.0 }, 5, 5, light_importance));

        // camera
        //camera cam(point3(0.0, 0.0, 30), point3(0.0, 0.0, 0.0), vec3(0, 1, 0), 45, aspect_ratio);
        camera cam(point3(5, 5.0, 30), point3(0.0, 0.0, 0.0), vec3(0, 1, 0), 45, aspect_ratio);

        // Render

        outdata << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        std::cerr << "\rScanlines remaining: " << image_height;// << "frame: " << i; //<< "of: " << total_frames;

        for (int j = image_height; j > 0; --j) {
            std::cerr << "\rScanlines remaining: " << j;// << ' ' << "frame: " << i << "of: " << total_frames << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0.0, 0.0, 0.0);
                for (int s = 0; s < samples_per_pixel; s++) {
                    auto u = double(i + random_double()) / image_width;
                    auto v = double(j + random_double()) / image_height;

                    //glm::mat4 identity = glm::identity<glm::mat4>();
                    //glm::mat4 rotation = glm::rotate(identity, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));

                    ray r = cam.get_ray(u, v);
                    pixel_color += ray_color(r, world, lights, max_bounce);
                }
                write_color(outdata, pixel_color, samples_per_pixel);
            }
        }

        std::cerr << "\nDone.\n";
        //i++;
        //refractive += 0.02;
    //}
}