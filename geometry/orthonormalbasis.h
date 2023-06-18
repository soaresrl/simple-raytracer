#ifndef ORTHO_BASIS_H
#define ORTHO_BASIS_H

#include "vec3.h"

class OrthonormalBasis
{
public:
	OrthonormalBasis(){}
	
	vec3 u() const { return axis[0]; }
	vec3 v() const { return axis[1]; }
	vec3 w() const { return axis[2]; }

	vec3 local(double a, double b, double c) const
	{
		return a * axis[0] + b * axis[1] + c * axis[2];
	}

	vec3 local(const vec3& p) const
	{
		return p.x() * axis[0] + p.y() * axis[1] + p.z() * axis[2];
	}

	void build_from_w(const vec3& w)
	{
		axis[2] = unit_vector(w);

		vec3 a = fabs(w.x()) > 0.9 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0);

		axis[1] = unit_vector(cross(w, a));
		axis[0] = unit_vector(cross(axis[2], axis[1]));
	}

public:
	vec3 axis[3];
};

#endif // !ORTHO_BASIS_H
