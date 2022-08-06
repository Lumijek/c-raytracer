#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct{
	point3 origin;
	vec3 direction;
} ray;

point3 rayat(const ray *r, const double t);

#endif