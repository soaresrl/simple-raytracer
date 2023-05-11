#ifndef RAYCOLOR_H
#define RAYCOLOR_H

#include "geometry/vec3.h"
#include "geometry/color.h"
#include "objects/scene.h"
#include "objects/light.h"

color computeDiffuseComponent(point3 point, point3 camera_pos, vec3 normal, scene& _scene) {
    color light_contribution(0.0, 0.0, 0.0);

    // ambient
    color ambient_light(0.2, 0.2, 0.2);

    //light_contribution += ambient_light;

    for (std::shared_ptr<Light> light : _scene.lights)
    {
        light_contribution += light->calc_diffuse_component(camera_pos, point, normal, *_scene.world);
    }

    return light_contribution;
}

color computeSpecularComponent(point3 point, point3 camera_pos, vec3 normal, int specular, scene& _scene) {
    color light_contribution(0.0, 0.0, 0.0);

    // ambient
    color ambient_light(0.2, 0.2, 0.2);

    //light_contribution += ambient_light;

    for (std::shared_ptr<Light> light : _scene.lights)
    {
        light_contribution += light->calc_specular_component(camera_pos, point, normal, specular, *_scene.world);
    }

    return light_contribution;
}

color computeLighting(point3 point, point3 camera_pos, vec3 normal, int specular, scene& _scene) {
    color light_contribution(0.0, 0.0, 0.0);

    // ambient
    color ambient_light(0.2, 0.2, 0.2);

    //light_contribution += ambient_light;

    for (std::shared_ptr<Light> light : _scene.lights)
    {
        light_contribution += light->calc_contribution(camera_pos, point, normal, specular, *_scene.world);
    }

    return light_contribution;
}

//color ray_color(const ray& r, const scene& scene, int depth) {
//    hit_record rec;
//
//    if (depth <= 0)
//        return color(0, 0, 0);
//
//    if (scene.world->hit(r, 0.001, infinity, rec)) {
//        ray scattered;
//        color attenuation;
//        if (rec.material_ptr->scatter(r, rec, attenuation, scattered))
//            return attenuation * ray_color(scattered, scene, depth - 1);
//        return color(0, 0, 0);
//    }
//
//    vec3 unit_direction = unit_vector(r.direction());
//    auto t = 0.5 * (unit_direction.y() + 1.0);
//    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//}

color raycolor(const ray& r, scene& _scene, double t_min, double t_max/*, int depth*/, int recursion_depth) {
    hit_record rec;

    //if (depth <= 0) return color(0, 0, 0);

    if (recursion_depth <= 0) {
        return color{ 0.0, 0.0, 0.0 };
    }

    if (_scene.world->hit(r, t_min, t_max, rec)) {
        //point3 target = rec.p + rec.normal /* + random_in_unit_sphere() */;

        //color local_color = rec.col * computeLighting(rec.p, r.origin(), rec.normal, rec.specular, _scene);
        color material_color;
        color diffuse_component = rec.col * computeDiffuseComponent(rec.p, r.origin(), rec.normal, _scene);
        color specular_component = rec.col * computeSpecularComponent(rec.p, r.origin(), rec.normal, rec.specular, _scene);

        if (rec.reflective > 0) {
            vec3 reflected_ray_direction = 2 * rec.normal * dot(rec.normal, -r.direction()) + r.direction();
            ray reflected_ray(rec.p, reflected_ray_direction);

            color reflected_color = raycolor(reflected_ray, _scene, 0.001, infinity, recursion_depth - 1);

            material_color = diffuse_component * (1 - rec.reflective) + reflected_color * rec.reflective;
        }
        
        if (rec.refractive_index <= 0) {
            return material_color + specular_component + rec.col * 0.2;
        }
        
        color refracted_color;

        vec3 incident = r.direction() / r.direction().length();

        float refrac_ratio = rec.is_front_face ? 1.0 / rec.refractive_index : rec.refractive_index;

        double c1 = 1.0f - (pow(refrac_ratio, 2)) * (1.0 - pow(dot(-incident, rec.normal), 2));

        if (c1 < 0.0) {
            refracted_color = color(0.0, 0.0, 0.0);
        }
        else {
            vec3 refracted_ray_direction = refract(incident, rec.normal, refrac_ratio);
            ray refracted_ray(rec.p, refracted_ray_direction);

            refracted_color = raycolor(refracted_ray, _scene, 0.001, infinity, recursion_depth - 1);
        }

        material_color = rec.translucency * refracted_color + (material_color + rec.col * 0.2) * (1.0 - rec.translucency);

        return material_color + specular_component;
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

#endif // !RAYCOLOR_H
