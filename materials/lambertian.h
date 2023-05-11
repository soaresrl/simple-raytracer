#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "materials/material.h"
#include "geometry/vec3.h"

class Lambertian : public Material
{
public:
    Lambertian(const color& materialColor) : albedo(materialColor) {}

    virtual bool scatter(
        const ray& in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        /*if (scatter_direction.near_zero())
            scatter_direction = rec.normal;*/

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    color albedo;
};

#endif // !LAMBERTIAN_H