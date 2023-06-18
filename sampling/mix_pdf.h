#ifndef MIXTURE_PDF_H
#define MIXTURE_PDF_H

#include <memory>

#include "pdf.h"

#include "utils/utility.h"

using namespace std;

class MixturePDF : public PDF
{
public:
	MixturePDF(shared_ptr<PDF> firstPDF, shared_ptr<PDF> secondPDF)
	{
		pdfs[0] = firstPDF;
		pdfs[1] = secondPDF;
	}

	virtual double value(const vec3& direction) const override
	{
		return 0.5 * pdfs[0]->value(direction) + 0.5 * pdfs[1]->value(direction);
	}

	virtual vec3 generate() const override
	{
		return random_double() < 0.5 ? pdfs[0]->generate() : pdfs[1]->generate();
	}

public:
	shared_ptr<PDF> pdfs[2];
};

#endif // !MIXTURE_PDF_H
