#include <iostream>

#include "utils/utility.h"
#include "geometry/color.h"
#include "objects/hittable_list.h"
#include "objects/sphere.h"
#include "objects/scene.h"
#include "objects/camera.h"
#include "objects/point_light.h"
#include "objects/directional_light.h"
#include "objects/light.h"

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

color computeLighting(point3 point, point3 camera_pos, vec3 normal, int specular, scene& world) {
    color light_contribution(0.0, 0.0, 0.0);

    // ambient
    color ambient_light(0.2, 0.2, 0.2);

    light_contribution += ambient_light;

    light_contribution += world.calc_light(camera_pos, point, normal, specular);

    return light_contribution;
}

/**
 * Essa função irá fazer um gradiente entre branco e azul
 * para que sirva como o background quando um raio não
 * colidir com nenhum objeto
 * caso colida, a cor do pixel será vermelha

*/
color raycolor(const ray& r, scene& world, double t_min, double t_max/*, int depth*/, int recursion_depth) {
    hit_record rec;

    //if (depth <= 0) return color(0, 0, 0);

    if (world.hit(r, t_min, t_max, rec)) {
        point3 target = rec.p + rec.normal /* + random_in_unit_sphere() */;

        color local_color = rec.col * computeLighting(rec.p, r.origin(), rec.normal, rec.specular, world);

        if (recursion_depth <= 0 || rec.reflective <= 0) {
            return local_color;
        }

        vec3 reflected_ray_direction = 2 * rec.normal * dot(rec.normal, - r.direction()) + r.direction();
        ray reflected_ray(rec.p, reflected_ray_direction);
        
        color reflected_color = raycolor(reflected_ray, world, 0.001, infinity, recursion_depth - 1);

        return local_color * (1 - rec.reflective) + reflected_color * rec.reflective;
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    // Image Properties
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_bounce = 50;

    // world 
    scene world;

    world.add_object(make_shared<sphere>(point3(0.0, -1.0, -1.0), 1.0, color(1.0, 0.0, 0.0), 500, 0.2));
    world.add_object(make_shared<sphere>(point3(-2.0, 0.0, -2.0), 1.0, color(0.0, 1.0, 0.0), 500, 0.5));
    world.add_object(make_shared<sphere>(point3(2.0, 0.0, -2.0), 1.0, color(0.0, 0.0, 1.0), 500, 0.2));
    world.add_object(make_shared<sphere>(point3(0.0, -5001.0, -1.0), 5000.0, color(1.0, 1.0, 0.0), 500, 0.1));


    world.add_light(make_shared<point_light>(point3(-0.0, 2.0, -1.0), color(1.0, 1.0, 1.0)));
    //world.add_light(make_shared<point_light>(point3(3.0, 3.0, -1.0), color(1.0, 1.0, 1.0)));
    world.add_light(make_shared<directional_light>(color(1.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0)));

    // camera
    camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height / 2; j >= - image_height / 2; --j) {
        std::cerr << "\rScanlines remaining: " << j + image_height / 2 << ' ' << std::flush;
        for (int i = -image_width/2; i < image_width/2; ++i) {
            color pixel_color(0.0, 0.0, 0.0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = double(i + random_double()) / image_width;
                auto v = double(j + random_double()) / image_height;

                ray r = cam.get_ray(u, v);
                pixel_color += raycolor(r, world, 0.001, infinity, 5);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}