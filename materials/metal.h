#ifndef METAL_H
#define METAL_H

#include "materials/material.h"

class Metal : public Material
{
public:
    Metal(const color& materialColor, float _fuzz) : albedo(materialColor), fuzz(_fuzz) {}

    virtual bool scatter(
        const ray& in, const hit_record& rec, scatter_record& srec
    ) const override {
        vec3 reflected = reflect(unit_vector(in.direction()), rec.normal);
        srec.specular= ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        srec.attenuation = albedo;
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;

        return (dot(srec.specular.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    float fuzz;
};

#endif // !LAMBERTIAN_H
