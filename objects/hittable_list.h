#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

#include "utils/utility.h"

#include "light.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
	public:
		hittable_list(){}
		hittable_list(shared_ptr<hittable> obj) { add(obj); };

		void clear() { objects.clear(); }
		void add(shared_ptr<hittable> obj) { objects.push_back(obj); }

		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

		virtual double pdf_value(const point3& o, const vec3& v) const override
		{
			float weight = 1.0 / objects.size();

			float sum = 0.0;

			for (auto& obj : objects)
			{
				sum += weight * obj->pdf_value(o, v);
			}

			return sum;
		}

		virtual vec3 random(const vec3& o) const {
			auto int_size = static_cast<int>(objects.size());

			return objects[random_int(0, int_size - 1)]->random(o);
		}

	public:
		std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	double closest = t_max;

	for (const auto& object : objects) {
		if (object->hit(r, t_min, closest, temp_rec)) {
			hit_anything = true;
			closest = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

#endif // !HITTABLE_LIST_H
