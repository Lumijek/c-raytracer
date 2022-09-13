#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "mvec3.h"
#include "material.h"

typedef struct s sphere;
typedef struct s{
	point3 center;
	double radius;
	material *mat_ptr;
	//bool (*hit)(sphere, const ray*, double, double, hit_record*);

} sphere;

bool hit_sphere(const sphere s, const ray* r, double t_min, double t_max, hit_record *rec);
#endif