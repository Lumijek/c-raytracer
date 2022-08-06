#include "ray.h"

point3 rayat(const ray *r, const double t) {
	return add(r->origin, scale(r->direction, t));
}