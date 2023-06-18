#ifndef PDF_H
#define PDF_H

#include "geometry/vec3.h"

class PDF
{
public:
	virtual double value(const vec3& direction) const = 0;
	virtual vec3 generate() const = 0;
};

#endif // !PDF_H
