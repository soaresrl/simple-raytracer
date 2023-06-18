#ifndef RAYCOLOR_H
#define RAYCOLOR_H

#include "geometry/vec3.h"
#include "geometry/color.h"

#include "objects/scene.h"
#include "objects/light.h"

#include "materials/material.h"

#include "sampling/cosine_pdf.h"
#include "sampling/mix_pdf.h"
#include "sampling/hittable_pdf.h"

color ray_color(const ray& r, const scene& scene, shared_ptr<hittable> direct_sample, int depth) {
    hit_record rec;
    scatter_record srec;

    if (depth <= 0)
        return color(0, 0, 0);

    if (!scene.world->hit(r, 0.001, infinity, rec)) return color{ 0.0, 0.0, 0.0 };

    ray scattered;

    color emmited = rec.material_ptr->emitted(0.0, 0.0, rec, rec.p);
    
    if (!rec.material_ptr->scatter(r, rec, srec)) return emmited;

    if (srec.is_specular) return srec.attenuation * ray_color(srec.specular, scene, direct_sample, depth - 1);

    shared_ptr<HittablePDF> lights_pdf_ptr = make_shared<HittablePDF>(direct_sample, rec.p);

    MixturePDF mix_pdf(lights_pdf_ptr, srec.pdf_ptr);

    scattered = ray(rec.p, mix_pdf.generate());

    double pdf_value = mix_pdf.value(scattered.direction());

    return emmited + srec.attenuation * rec.material_ptr->scattering_pdf(r, rec, scattered) * ray_color(scattered, scene, direct_sample, depth - 1) / pdf_value;
}

#endif // !RAYCOLOR_H
