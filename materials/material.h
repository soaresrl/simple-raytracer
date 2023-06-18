#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>

#include "geometry/ray.h"

#include "sampling/pdf.h"

using namespace std;

struct hit_record;

struct scatter_record {
	ray specular;
	bool is_specular;
	color attenuation;
	shared_ptr<PDF> pdf_ptr;
};

class Material
{
public:
	virtual bool scatter(const ray& in, const hit_record& rec, scatter_record& scatter_rec) const
	{
		return false;
	}

	virtual double scattering_pdf(const ray& in, const hit_record& rec, const ray& scattered) const
	{
		return 0.0f;
	}

	virtual color emitted(float u, float v, const hit_record& rec, const point3& p) const { return color{ 0.0f, 0.0f, 0.0f }; }
};


#endif // !MATERIAL_H
