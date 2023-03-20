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
	// Retorna um número real [0, 1)
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	// Retorna um número real [min, max)
	return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;

	return x;
}

#include "geometry/ray.h"
#include "geometry/vec3.h"

#endif // !UTILITY_H
