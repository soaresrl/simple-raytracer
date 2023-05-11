#ifndef SCENE_H
#define SCENE_H

#include "objects/hittable.h"
#include "objects/hittable_list.h"
#include "objects/light.h"

class scene {
public:
	scene() 
	{
		world = new hittable_list();
	}
	scene(shared_ptr<hittable> obj) { 
		world = new hittable_list();

		add_object(obj); 
	};

	//void clear() { objects.clear(); }
	void add_object(shared_ptr<hittable> obj) { world->add(obj); }

	void add_light(shared_ptr<Light> light_obj) { lights.push_back(light_obj); }

	//virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

	//color calc_light(point3 camera_pos, point3 _point, vec3 normal, int specular);

public:
	//std::vector<shared_ptr<hittable>> objects;
	hittable_list* world;
	std::vector<shared_ptr<Light>> lights;

};


//bool scene::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
//	hit_record temp_rec;
//	bool hit_anything = false;
//	double closest = t_max;
//
//	for (const auto& object : objects) {
//		if (object->hit(r, t_min, closest, temp_rec)) {
//			hit_anything = true;
//			closest = temp_rec.t;
//			rec = temp_rec;
//		}
//	}
//
//	return hit_anything;
//}

//color scene::calc_light(point3 camera_pos, point3 _point, vec3 normal, int specular) {
//	color contribution(0.0, 0.0, 0.0);
//
//	for (auto& light_obj : lights) {
//		contribution += light_obj->calc_contribution(camera_pos, _point, normal, specular, *this);
//	}
//
//	return contribution;
//}

#endif // !SCENE_H
