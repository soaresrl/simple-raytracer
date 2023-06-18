#ifndef FLIP_FACE_H
#define FLIP_FACE_H

#include <memory>

#include "hittable.h"

using namespace std;

class FlipFace : public hittable
{
public:
	FlipFace(shared_ptr<hittable> ptr) : geom_ptr(ptr) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override
	{
		if (!geom_ptr->hit(r, t_min, t_max, rec)) return false;

		rec.is_front_face = !rec.is_front_face;

		return true;
	}

private:
	shared_ptr<hittable> geom_ptr;
};
#endif // !FLIP_FACE_H
