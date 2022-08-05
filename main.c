#include "utils/vec3.h"
#include "utils/ray.h"
#include "utils/color.h"
#include "utils/sphere.h"
#include "utils/hittable.h"
#include "utils/hittable_list.h"
#include "utils/camera.h"
#include "utils/utility.h"

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
		point3 target = add(add(rec.p, rec.normal), random_in_unit_sphere());
		ray rr = {rec.p, sub(target, rec.p)};
		return scale(ray_color(&rr, world, depth - 1), 0.5);
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
	const int samples_per_pixel = 10;
	const int max_depth = 2;

	// World
	hittable_list world;

	world.current_index = 0;
	add_obj(&world, (sphere){p3(0, 0, -1), 0.5, hit_sphere});
	add_obj(&world, (sphere){p3(0, -100.5, -1), 100, hit_sphere});

	// Camera

	camera cam;
	setup_camera(&cam);

	clock_t begin = clock();
	printf("P3\n%d %d\n255\n", image_width, image_height);

	for(int j = image_height - 1; j >= 0; j--) {
		//fprintf(stderr, "%d iterations remaining!\n", j);
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