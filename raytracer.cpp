#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
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

scene cornell_box() {
    scene objects;

    auto red = make_shared<Lambertian>(color(.65, .05, .05));
    auto white = make_shared<Lambertian>(color(.73, .73, .73));
    auto green = make_shared<Lambertian>(color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(color(15, 15, 15));

    objects.add_object(make_shared<Triangle>(point3{ -3.0, 0.0, 0.0 }, point3{ 0.0, 5.0, 0.0 }, point3{ 3.0, 0.0, 0.0 }, red));
    objects.add_object(make_shared<YZ_Plane>(point3{ -5, 0.0, 0.0 }, 10, 10, green));
    objects.add_object(make_shared<YZ_Plane>(point3{ 5, 0.0, 0.0 }, 10, 10, red));
    objects.add_object(make_shared<FlipFace>(make_shared<XZ_Plane>(point3{ 0.0, 4.995, 0.0 }, 5, 5, light)));
    objects.add_object(make_shared<XZ_Plane>(point3{ 0.0, 5, 0.0 }, 10, 10, white));
    objects.add_object(make_shared<XZ_Plane>(point3{ 0.0, -5, 0.0 }, 10, 10, white));
    objects.add_object(make_shared<XY_Plane>(point3{ 0.0, 0.0, -5 }, 10, 10, white));

    auto dielectric = make_shared<Dielectric>(1.75);
    auto metal = make_shared<Metal>(color{ 1.0, 1.0, 1.0 }, 0.5);
    objects.add_object(make_shared<sphere>(point3{ 0.0, -3.5, 0.0 }, 1.5, dielectric));
    objects.add_object(make_shared<sphere>(point3{ -3.0, -3.5, 0.0 }, 1.5, red));
    objects.add_object(make_shared<sphere>(point3{ 3.0, -3.5, 0.0 }, 1.5, metal));

    return objects;
}

/** 
 * Essa função calcula se um raio colide com uma esfera
 * vale relembrar que pode possuir dois casos, com:
 * - 0 raízes reais - não colide com a esfera
 * - 1 raíz real - colide com a esfera de forma tangente
 * - 2 raízes reais - colide com a esfera passando de um lado para o outro
 * 
 * para solucionar é necessário resolver a equação do segundo grau
 * (b.b)*t^2 + (2.b.(A-C)).t + (A-C).(A-C) - r^2 = 0
 * 
 * onde b é o vetor que dá direção ao raio, A a origem do raio
 * e C o centro da esfera, e r o raio da esfera.
*/
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 origin_to_center = r.origin() - center;

    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(origin_to_center, r.direction());
    auto c = dot(origin_to_center, origin_to_center) - (radius * radius);

    auto delta = (b * b) - (4 * a * c);

    if (delta < 0) {
        return -1.0;
    }
    
    return (-b - sqrt(delta)) / (2.0 * a);
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
        std::ofstream outdata("08_06_cornell_box.ppm"/* + std::to_string(refractive) + ".ppm"*/);

        // world 
        // scene world = cornell_box();

        scene world;

            
        auto objs = FromOBJ("./assets/ob1.obj");

        for (auto& obj : objs)
        {
            world.add_object(obj);
        }

        auto white = make_shared<Lambertian>(color(.73, .73, .73));

        world.add_object(make_shared<XZ_Plane>(point3{ 0.0, -18, 0.0 }, 36, 36, white));
        world.add_object(make_shared<XZ_Plane>(point3{ 0.0, 18, 0.0 }, 36, 36, white));
        world.add_object(make_shared<YZ_Plane>(point3{ -18, 0.0, 0.0 }, 36, 36, white));
        world.add_object(make_shared<YZ_Plane>(point3{ 18, 0.0, 0.0 }, 36, 36, white));
        world.add_object(make_shared<XY_Plane>(point3{ 0.0, 0.0, -10.0 }, 36, 36, white));

        auto light = make_shared<DiffuseLight>(color(15, 15, 15));
        world.add_object(make_shared<FlipFace>(make_shared<XZ_Plane>(point3{ 0.0, 17.995, 0.0 }, 15, 15, light)));

        auto lights = make_shared<hittable_list>();
        auto light_importance = make_shared<DiffuseLight>(color(15, 15, 15));

        lights->add(make_shared<XZ_Plane>(point3{ 0.0, 17.995, 0.0 }, 15, 15, light_importance));

        // camera
        camera cam(point3(0.0, 18, 100), point3(0, 6.0, 0.0), vec3(0, 1, 0), 45, aspect_ratio);

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