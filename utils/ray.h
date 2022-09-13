#ifndef RAY_H
#define RAY_H

#include "mvec3.h"

typedef struct{
	point3 origin;
	mvec3 direction;
} ray;

point3 rayat(const ray *r, const double t);

#endif