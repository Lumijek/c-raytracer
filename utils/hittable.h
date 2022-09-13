#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "material.h"
#include <stdbool.h>

struct material;
typedef struct hit_record{
	point3 p;
	mvec3 normal;
	struct material* mat_ptr;
	double t;
	bool front_face;

} hit_record;

void set_face_normal(hit_record *rec, const ray *r, mvec3 outward_normal);


#endif