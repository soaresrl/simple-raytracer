#ifndef COSINE_PDF_H
#define COSINE_PDF_H

#include "pdf.h"

#include "geometry/orthonormalbasis.h"

#include "utils/utility.h"

class CosinePDF : public PDF
{
public:
	CosinePDF(const vec3& w) 
	{
		basis.build_from_w(w);
	}

	virtual double value(const vec3& direction) const override
	{
		double cos = dot(basis.w(), direction);

		return cos < 0.0 ? 0.0 : cos / pi;
	}

	virtual vec3 generate() const override
	{
		return basis.local(random_cosine_direction());
	}

public:
	OrthonormalBasis basis;
};

#endif // !COSINE_PDF_H
