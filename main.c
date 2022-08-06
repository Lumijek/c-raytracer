#include "utils/vec3.h"
#include "utils/ray.h"
#include "utils/color.h"
#include "utils/sphere.h"
#include "utils/hittable.h"
#include "utils/hittable_list.h"
#include "utils/camera.h"
#include "utils/utility.h"
#include "utils/hittable_types.h"
#include "utils/material.h"
#include "utils/material_types.h"

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

typedef struct pixel{
	int r;
	int g;
	int b;

} pixel;
color ray_color(const ray* r, const hittable_list *world, int depth){
	hit_record rec;

	if (depth <= 0) {
		return col(0, 0, 0);
	}
	if (hit(world, r, 0.0001, 10000, &rec)){
		ray scattered;
		color attenuation;
		if (scatter(rec.mat_ptr, r, &rec, &attenuation, &scattered)) {
			return mul(ray_color(&scattered, world, depth-1), attenuation);
		}
		return (color){0, 0, 0};
	}
	vec3 unit_direction = unit_vector(r->direction);
	double t = 0.5 * (unit_direction.y + 1.0);
	return add(scale(col(1.0, 1.0, 1.0), (1.0 - t)), scale(col(0.5, 0.7, 1.0), t));
}
int main() {

	// Image

	const float aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = (int) (image_width / aspect_ratio);
	const int samples_per_pixel = 75;
	const int max_depth = 50;

	// World
	hittable_list world;

	material material_ground = (material) {LAMBERTIAN, (color) {0.8, 0.8, 0.0}};
	material material_center = (material) {LAMBERTIAN, (color) {0.1, 0.2, 0.5}};
	material material_left = (material) {DIELECTRIC, .d = 1.5};
	material material_right = (material) {METAL, .m = {(color){0.8, 0.6, 0.2}, 0.0}};

	world.current_index = 0;
	add_obj(&world, (hittable) {.type = SPHERE, .s = (sphere){p3(0.0, -100.5, -1), 100, &material_ground}});
	add_obj(&world, (hittable) {.type = SPHERE, .s = (sphere){p3(0.0, 0.0, -1), 0.5, &material_center}});
	add_obj(&world, (hittable) {.type = SPHERE, .s = (sphere){p3(-1.0, 0.0, -1), 0.5, &material_left}});
	add_obj(&world, (hittable) {.type = SPHERE, .s = (sphere){p3(-1.0, 0.0, -1), -0.4, &material_left}});
	add_obj(&world, (hittable) {.type = SPHERE, .s = (sphere){p3(1.0, 0.0, -1), 0.5, &material_right}});

	// Camera

	camera cam;
	setup_camera(&cam);

	clock_t begin = clock();
	printf("P3\n%d %d\n255\n", image_width, image_height);

	for(int j = image_height - 1; j >= 0; j--) {
		fprintf(stderr, "%d iterations remaining!\n", j);
		for(int i = 0; i < image_width; i++) {
			color pixel_color = col(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++) {
				double u = (double) (((double)(i) + random_double()) / (image_width - 1));
				double v = (double) (((double)(j) + random_double()) / (image_height - 1));
				ray r = get_ray(&cam, u, v);
				iadd(&pixel_color, ray_color(&r, &world, max_depth));
			}
			write_color(pixel_color, samples_per_pixel);
		}
	}
	clock_t end = clock();
	fprintf(stderr, "Time spent: %f seconds!\n", (double)(end - begin) / CLOCKS_PER_SEC);
}