#include "material.h"

static double reflectance(double cosine, double ref_idx) {
	double r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1-r0)*pow((1-cosine), 5);
}
bool lambertian_scatter(lambertian lam, const ray* r_in, const struct hit_record* rec, color* attenuation, ray* scattered) {
	mvec3 scatter_direction = add(rec->normal, random_unit_vector());
	if (near_zero(scatter_direction)) {
		scatter_direction = rec->normal;
	}
	*scattered = (ray){rec->p, scatter_direction};
	*attenuation = lam.albedo;
	return true;
}

bool metal_scatter(metal m, const ray* r_in, const struct hit_record *rec, color *attenuation, ray* scattered) {
	mvec3 reflected = reflect(unit_vector(r_in->direction), rec->normal);
	*scattered = (ray){rec->p, add(reflected, scale(random_in_unit_sphere(), m.fuzz))};
	*attenuation = m.albedo;
	return (dot(scattered->direction, rec->normal) > 0);
}

bool dielectric_scatter(dielectric d, const ray* r_in, const struct hit_record *rec, color *attenuation, ray* scattered) {
	*attenuation = (color) {1.0, 1.0, 1.0};
	double refraction_ratio = rec->front_face ? (1.0 / d.ir) : d.ir;

	mvec3 unit_direction = unit_vector(r_in->direction);
	double cos_theta = fmin(dot(negate(unit_direction), rec->normal), 1.0);
	double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	bool cannot_refract = refraction_ratio * sin_theta > 1.0;
	mvec3 direction;

	if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
		direction = reflect(unit_direction, rec->normal);
	}
	else {
		direction = refract(unit_direction, rec->normal, refraction_ratio);
	}
	*scattered = (ray) {rec->p, direction};
	return true;
}
bool scatter(material *mat, const ray* r_in, const struct hit_record *rec, color *attenuation, ray* scattered) {
	switch(mat->type) {
		case LAMBERTIAN:
			return lambertian_scatter(mat->lam, r_in, rec, attenuation, scattered);

		case METAL:
			return metal_scatter(mat->m, r_in, rec, attenuation, scattered);

		case DIELECTRIC:
			return dielectric_scatter(mat->d, r_in, rec, attenuation, scattered);

	}
	return false;
}

material *initialize_lambertian(color albedo) {
	material *mat = (material *)malloc(sizeof(material));
	mat->type = LAMBERTIAN;
	mat->lam = (lambertian) {albedo};
	return mat;
}
material *initialize_metal(color albedo, double fuzz) {
	material *mat = (material *) malloc(sizeof(material));
	mat->type = METAL;
	mat->m = (metal) {albedo, fuzz};
	return mat;
}
material *initialize_dielectric(double ir) {
	material *mat = (material *) malloc(sizeof(material));
	mat->type = DIELECTRIC;
	mat->d = (dielectric) {ir};
	return mat;
}