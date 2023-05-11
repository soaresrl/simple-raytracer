#ifndef MATERIAL_H
#define MATERIAL_H

#include "geometry/ray.h"

struct hit_record;

class Material
{
public:
	virtual bool scatter(const ray& in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;

};


#endif // !MATERIAL_H
