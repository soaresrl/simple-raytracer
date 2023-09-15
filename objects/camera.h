#ifndef CAMERA_H
#define CAMERA_H

#include "utils/utility.h"

class camera {
public:
	camera(point3 lookfrom, point3 lookat, vec3 vup, float vfov, float _aspect_ratio)  {
		aspect_ratio = _aspect_ratio;
		fov = vfov;
		v_h = tan((45.0 * pi / 180) / 2);
		v_w = aspect_ratio * v_h;
		focal_length = 1.0;

		vec3 w = unit_vector(lookfrom - lookat);
		vec3 u = unit_vector(cross(vup, w));
		vec3 v = cross(w, u);

		//origin = point3(0.0, 0.0, 5.0);
		//horizontal = vec3(v_w, 0.0, 0.0);
		//vertical = vec3(0.0, v_h, 0.0);	

		origin = lookfrom;
		horizontal = v_w * u;
		vertical = v_h * v;

		lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
	}

	/**
	* lower_left_corner + u * horizontal + v * vertical - origin
	* Ao percorrer a primeira iteração do loop interno o raio
	* sairá da origem ao ponto A na próxima iteração o raio saíra
	* da origem ao ponto B
	*
	* *-*-*-*-*
	* | | | | |
	* *-*-*-*-*
	* | | | | |
	* *-*-*-*-*
	* | | | | |
	* A-B-*-*-*
	*
	*/
	ray get_ray(double u, double v) const {
		//vec3 direction = point3(u * v_w, v * v_h, -focal_length + origin.z()) - origin;
		//vec3 direction = point3(u * v_w + origin.x(), v * v_h + origin.y(), -focal_length + origin.z()) - origin;

		//glm::vec4 aux_vec(direction.x(), direction.y(), direction.z(), 1.0f);
		//glm::vec4 rotated_direction = rotation * aux_vec;

		//return ray(origin, vec3(rotated_direction.x, rotated_direction.y, rotated_direction.z));
		return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

private:
	point3 origin;
	vec3 horizontal;
	vec3 vertical;
	float v_w;
	float v_h;
	float aspect_ratio;
	float focal_length;
	float fov;
	point3 lower_left_corner;
};

#endif // !CAMERA_H
