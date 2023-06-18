#ifndef DIFFUSE_LIGHT
#define DIFFUSE_LIGHT

#include <memory>

#include "geometry/color.h"
#include "materials/material.h"

using namespace std;

class DiffuseLight : public Material
{
public:
	DiffuseLight(color c) : lightColor(c) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, scatter_record& srec
	) const override {
		return false;
	}

	virtual color emitted(float u, float v, const hit_record& rec, const point3& p) const override {
		return rec.is_front_face ? lightColor : color{ 0.0, 0.0, 0.0 };
	}

private:
	color lightColor;
};

#endif // !DIFFUSE_LIGHT
