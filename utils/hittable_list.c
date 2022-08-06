#include "hittable.h"
#include "sphere.h"
#include "hittable_types.h"
#include "hittable_list.h"
#include <string.h>

bool hit_scene(const hittable obj, const ray* r, double t_min, double t_max, hit_record *rec) {
	switch(obj.type) {
		case SPHERE:
			return hit_sphere(obj.s, r, t_min, t_max, rec);
	}
	return false;
}
static inline void clear(hittable_list *world){
	memset(world->objects, 0, sizeof(world->objects));
}

void add_obj(hittable_list *world, hittable object) {
	switch(object.type) {
		case SPHERE:
			world->objects[world->current_index++] = object;
	}
}

bool hit(const hittable_list *world, const ray *r, double t_min, double t_max, hit_record *rec){
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;

	for(int i = 0; i < world->current_index; i++) {
		//if(world->objects[i].hit(world->objects[i], r, t_min, closest_so_far, &temp_rec)){
		if(hit_scene(world->objects[i], r, t_min, closest_so_far, &temp_rec)){
			hit_anything = true;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
	}
	return hit_anything;
}