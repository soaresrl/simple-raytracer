#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "materials/material.h"

#include "sampling/cosine_pdf.h"

#include "geometry/vec3.h"

class Lambertian : public Material
{
public:
    Lambertian(const color& materialColor) : albedo(materialColor) {}

    virtual bool scatter(
        const ray& in, const hit_record& rec, scatter_record& srec
    ) const override {
        srec.is_specular = false;
        srec.attenuation = albedo;
        srec.pdf_ptr = make_shared<CosinePDF>(rec.normal);

        return true;
    }

    virtual double scattering_pdf(const ray& in, const hit_record& rec, const ray& scattered) const override
    {
        double cos = dot(rec.normal, scattered.direction()) / (rec.normal.length() * scattered.direction().length());
        
        return cos < 0.0 ? 0.0 : cos / pi;
    }

public:
    color albedo;
};

#endif // !LAMBERTIAN_H
