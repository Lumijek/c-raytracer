#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

typedef struct s sphere;
typedef struct s{
	point3 center;
	double radius;
	bool (*hit)(sphere, const ray*, double, double, hit_record*);

} sphere;

sphere create_sphere(point3 cen, double r) {
	sphere s = {cen, r};
	return s;
}

bool hit_sphere(const sphere s, const ray* r, double t_min, double t_max, hit_record *rec) {
	vec3 oc = sub(r->origin, s.center);
	double a = length_squared(r->direction);
	double half_b = dot(oc, r->direction);
	double c = length_squared(oc) - s.radius * s.radius;
	double discriminant = half_b * half_b - a * c;
	if (discriminant < 0) {
		return false;
	}

	double sqrtd = sqrt(discriminant);
	double root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root) {
			return false;
		}
	}


	rec->t = root;
	rec->p = rayat(r, rec->t);
	vec3 outward_normal = mdiv(sub(rec->p, s.center), s.radius);
	set_face_normal(rec, r, outward_normal);

	return true;
}
#endif