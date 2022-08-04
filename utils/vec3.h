#ifndef VEC3_H
#define VEC3_H

#include "utility.h"


#include <math.h>
#include <stdio.h>
#include <stdbool.h>

struct vec{
	double x;
	double y;
	double z;
};

typedef struct vec vec3;
typedef struct vec color;
typedef struct vec point3;

static inline struct vec v3(double x, double y, double z){
	vec3 vector = {x, y, z};
	return vector;
}

static inline struct vec col(double x, double y, double z){
	color vector = {x, y, z};
	return vector;
}
static inline struct vec p3(double x, double y, double z){
	point3 vector = {x, y, z};
	return vector;
}
static inline struct vec negate(const struct vec v){
	return v3(-v.x, -v.y, -v.z);
}

static inline void iadd(struct vec *v, const struct vec u){
	v->x += u.x;
	v->y += u.y;
	v->z += u.z;
}

static inline struct vec add(const struct vec v, const struct vec u){
	return v3(v.x + u.x, v.y + u.y, v.z + u.z);
}

static inline struct vec sub(const struct vec v, const struct vec u){
	return v3(v.x - u.x, v.y - u.y, v.z - u.z);
}

static inline void iscale(struct vec*v, const double t){
	v->x *= t;
	v->y *= t;
	v->z *= t;

}

static inline struct vec scale(const struct vec v, const double t) {
	return v3(v.x * t, v.y * t, v.z *t);
}

static inline void imul(struct vec *v, const struct vec u) {
	v->x *= u.x;
	v->y *= u.y;
	v->z *= u.z;
}

static inline struct vec mul(const struct vec v, const struct vec u){
	return v3(v.x * u.x, v.y * u.y, v.z * u.z);
}


static inline void idiv(struct vec *v, const double t) {
	v->x /= t;
	v->y /= t;
	v->z /= t;
}

static inline struct vec mdiv(const struct vec v, const double t){
	return v3(v.x * (1 / t), v.y * (1 / t), v.z * (1 / t));
}

static inline double length_squared(const struct vec v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline double length(const struct vec v) {
	return sqrt(length_squared(v));
}

//UTILITY

static inline void print_vec(const struct vec v){
	fprintf(stderr, "(%f, %f, %f)\n", v.x, v.y, v.z);
}

static inline double dot(const struct vec v, const struct vec u) {
	return v.x * u.x + v.y * u.y + v.z * u.z;
}

static inline struct vec cross(const struct vec v, const struct vec u) {
	return v3(v.y * u.z - v.z * u.y,
				v.z * u.x - v.x * u.z,
				v.x * u.y - v.y * u.x);
}

static inline struct vec unit_vector(struct vec v){
	return mdiv(v, length(v));
}

static inline vec3 random_vec() {
        return v3(random_double(), random_double(), random_double());
}

static inline vec3 random_vec_mm(double min, double max) {
        return v3(random_double_mm(min, max), random_double_mm(min, max), random_double_mm(min, max));
}

static inline vec3 random_in_unit_sphere() {
	while(true) {
		vec3 p = random_vec_mm(-1, 1);
		if(length_squared(p) >= 1) continue;
		return p;
	}

}
#endif