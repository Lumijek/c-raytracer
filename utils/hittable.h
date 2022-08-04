#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"
#include <stdbool.h>

typedef struct {
	point3 p;
	vec3 normal;
	double t;
	bool front_face;

} hit_record;

static inline void set_face_normal(hit_record *rec, const ray *r, vec3 outward_normal) {
	rec->front_face = (dot(r->direction, outward_normal) < 0);
	rec->normal = rec->front_face ? outward_normal : negate(outward_normal);
}
#endif