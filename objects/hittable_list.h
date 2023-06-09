#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

#include "light.h"

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
	public:
		hittable_list(){}
		hittable_list(shared_ptr<hittable> obj) { add(obj); };

		void clear() { objects.clear(); }
		void add(shared_ptr<hittable> obj) { objects.push_back(obj); }

		void add_light(shared_ptr<Light> light_obj) { lights.push_back(light_obj); }


		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

	public:
		std::vector<shared_ptr<hittable>> objects;
		std::vector<shared_ptr<Light>> lights;
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
