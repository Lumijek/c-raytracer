#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "sphere.h"
#include "hittable_types.h"

#include <string.h>

typedef struct {
	int type;
	union {
		sphere s;
	};
} hittable;

typedef struct {
	int current_index;
	hittable objects[500];
} hittable_list;


bool hit_scene(const hittable obj, const ray* r, double t_min, double t_max, hit_record *rec);

static inline void clear(hittable_list *world);
void add_obj(hittable_list *world, hittable object);
bool hit(const hittable_list *world, const ray *r, double t_min, double t_max, hit_record *rec);
#endif