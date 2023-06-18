#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double deg) {
	return deg * pi / 180.0;
}

inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
	return static_cast<int>(random_double(min, max + 1));
}

inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;

	return x;
}

#include "geometry/ray.h"
#include "geometry/vec3.h"

inline vec3 random_cosine_direction() {
	double ksi1 = random_double();
	double ksi2 = random_double();
	auto z = sqrt(1 - ksi2);

	auto phi = 2 * pi * ksi1;
	auto x = cos(phi) * sqrt(ksi2);
	auto y = sin(phi) * sqrt(ksi2);

	return vec3(x, y, z);
}

#endif // !UTILITY_H
