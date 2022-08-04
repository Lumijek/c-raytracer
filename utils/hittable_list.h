#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "sphere.h"

#include <string.h>
typedef struct {
	sphere objects[50];
	int current_index;
} hittable_list;


static inline void clear(hittable_list *world){
	memset(world->objects, 0, sizeof(world->objects));
}

static inline void add_obj(hittable_list *world, sphere object) {
	world->objects[world->current_index++] = object;
}

static inline bool hit(const hittable_list *world, const ray *r, double t_min, double t_max, hit_record *rec){
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;

	for(int i = 0; i < world->current_index; i++) {
		if(world->objects[i].hit(world->objects[i], r, t_min, closest_so_far, &temp_rec)){
			hit_anything = true;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
	}
	return hit_anything;
}
#endif