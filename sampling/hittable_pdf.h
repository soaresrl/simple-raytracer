#ifndef HITTABLE_PDF_H
#define HITTABLE_PDF_H

#include <memory>

#include "pdf.h"

#include "objects/hittable.h"

#include "utils/utility.h"

using namespace std;

class HittablePDF : public PDF
{
public:
	HittablePDF(shared_ptr<hittable> h, const point3 o) : origin(o), hittable_ptr(h){}

	virtual double value(const vec3& direction) const override
	{
		return hittable_ptr->pdf_value(origin, direction);
	}

	virtual vec3 generate() const override
	{
		return hittable_ptr->random(origin);
	}

public:
	shared_ptr<hittable> hittable_ptr;
	point3 origin;
};

#endif // !HITTABLE_PDF_H
