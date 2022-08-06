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

void random_scene(hittable_list *world) {
	world->current_index = 0;
	
	material ground_material = (material) {LAMBERTIAN, .lam = col(0.5, 0.5, 0.5)};
	add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){p3(0.0, -1000, 0), 1000, &ground_material}});
	/*
	for(int a = -11; a < 11; a++) {
		for(int b = -11; b < 11; b++) {
			double choose_mat = random_double();
			point3 center = p3(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if (length(sub(center, p3(4, 0.2, 0)))> 0.9) {
				material sphere_material;

				if (choose_mat < 0.8) {
					color albedo = mul(random_vec(), random_vec());
					sphere_material = (material) {LAMBERTIAN, .lam = albedo};
					add_obj(&world, (hittable) {.type = SPHERE, .s = (sphere){center, 0.2, &sphere_material}});
				}
				else if (choose_mat < 0.95){
					color albedo = random_vec_mm(0.5, 0.1);
					double fuzz = random_double_mm(0, 0.5);
					sphere_material = (material) {METAL, .m = {albedo, fuzz}};
					add_obj(&world, (hittable) {.type = SPHERE, .s = (sphere){center, 0.2, &sphere_material}});
				}
				else {
					sphere_material = (material) {DIELECTRIC, .d = 1.5};
					add_obj(&world, (hittable) {.type = SPHERE, .s = (sphere){center, 0.2, &sphere_material}});				
				}

			}
		}
	}
	*/

	material ground_material = (material) {LAMBERTIAN, .lam = col(0.5, 0.5, 0.5)};
	add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){p3(0.0, -1000, 0), 1000, &ground_material}});
	
	material material1 = (material) {DIELECTRIC, .d = 1.5};
	add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){p3(0, 1, 0), 1.0, &material1}});

	material material2 = (material) {LAMBERTIAN, .lam = col(0.4, 0.2, 0.1)};
	add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){p3(-4, 1, 0), 1.0, &material2}});

	material material3 = (material) {METAL, .m = {(color){0.7, 0.6, 0.5}, 0.0}};
	add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){p3(4, 1, 0), 1.0, &material3}});

}
int main() {

	// Image

	const float aspect_ratio = 3.0 / 2.0;
	const int image_width = 400;
	const int image_height = (int) (image_width / aspect_ratio);
	const int samples_per_pixel = 40;
	const int max_depth = 50;

	// World
	hittable_list world;
	random_scene(&world);

	// Camera

	camera cam;

	point3 lookfrom = p3(13, 2, 3);
	point3 lookat = p3(0, 0, 0);
	vec3 vup = v3(0, 1, 0);
	double dist_to_focus = 10.0;
	double aperture = 0.1;
	setup_camera(&cam, lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

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