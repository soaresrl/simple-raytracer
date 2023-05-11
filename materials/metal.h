#ifndef METAL_H
#define METAL_H

#include "materials/material.h"

class Metal : public Material
{
public:
    Metal(const color& materialColor, float _fuzz) : albedo(materialColor), fuzz(_fuzz) {}

    virtual bool scatter(
        const ray& in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override {
        vec3 reflected = reflect(unit_vector(in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    float fuzz;
};

#endif // !LAMBERTIAN_H
