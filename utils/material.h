#ifndef MATERIAL_H
#define MATERIAL_H
#include "mvec3.h"
#include "ray.h"
#include "hittable.h"
#include "material_types.h"

#include <math.h>

struct hit_record;
typedef struct {
	color albedo;

} lambertian;

typedef struct {
	color albedo;
	double fuzz;
} metal;

typedef struct {
	double ir;
} dielectric;

typedef struct material{
	int type;
	union {
		lambertian lam;
		metal m;
		dielectric d;
	};
} material;

bool lambertian_scatter(lambertian lam, const ray* r_in, const struct hit_record* rec, color *attenuation, ray* scattered);
bool metal_scatter(metal m, const ray* r_in, const struct hit_record *rec, color *attenuation, ray* scattered);
bool dielectric_scatter(dielectric d, const ray* r_in, const struct hit_record *rec, color *attenuation, ray* scattered);
bool scatter(material *mat, const ray* r_in, const struct hit_record *rec, color *attenuation, ray* scattered);
material *initialize_lambertian(color albedo);
material *initialize_metal(color albedo, double fuzz);
material *initialize_dielectric(double ir);

#endif