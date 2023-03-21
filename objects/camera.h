#ifndef CAMERA_H
#define CAMERA_H

#include "utils/utility.h"
#include <glm/glm.hpp>

class camera {
public:
	camera()  {
		aspect_ratio = 16.0f / 9.0f;
		v_h = 2.0;
		v_w = aspect_ratio * v_h;
		focal_length = 1.0f;

		origin = point3(0, 0, 6);
		horizontal = vec3(v_w, 0.0, 0.0);
		vertical = vec3(0.0, v_h, 0.0);	
	}

	/**
	* lower_left_corner + u * horizontal + v * vertical - origin
	* Ao percorrer a primeira itera��o do loop interno o raio
	* sair� da origem ao ponto A na pr�xima itera��o o raio sa�ra
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
	ray get_ray(double u, double v, glm::mat4 rotation) const {
		vec3 direction = point3(u * v_w, v * v_h, -focal_length) - origin;

		glm::vec4 aux_vec(direction.x(), direction.y(), direction.z(), 1.0f);
		glm::vec4 rotated_direction = rotation * aux_vec;

		return ray(origin, vec3(rotated_direction.x, rotated_direction.y, rotated_direction.z));
	}

private:
	point3 origin;
	vec3 horizontal;
	vec3 vertical;
	float v_w;
	float v_h;
	float aspect_ratio;
	float focal_length;
};

#endif // !CAMERA_H
