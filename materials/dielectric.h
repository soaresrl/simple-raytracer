#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "materials/material.h"
#include "utils/utility.h"

class Dielectric : public Material
{
public:
    Dielectric(float _refractionIndex) : refractionIndex(_refractionIndex) {}

    virtual bool scatter(
        const ray& in, const hit_record& rec, scatter_record& srec
    ) const override {
        srec.attenuation = color(1.0, 1.0, 1.0);
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;

        float refraction_ratio = rec.is_front_face ? (1.0 / refractionIndex) : refractionIndex;

        vec3 unit_direction = unit_vector(in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        srec.specular = ray(rec.p, direction);
        return true;
    }

public:
    float refractionIndex;

private:
    static double reflectance(double cosine, double ref_idx) {
        double r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif // !DIELECTRIC_H
