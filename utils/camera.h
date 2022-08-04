#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"
typedef struct {
	float aspect_ratio;
	float viewport_height;
	float viewport_width;
	float focal_length;

	point3 origin;
	vec3 horizontal;
	vec3 vertical;
	vec3 lower_left_corner;
} camera;

void setup_camera(camera *cam) {
	cam->aspect_ratio = 16.0 / 9.0;
	cam->viewport_height = 2.0;
	cam->viewport_width = cam->aspect_ratio * cam->viewport_height;
	cam->focal_length = 1.0;

	cam->origin = p3(0, 0, 0);
	cam->horizontal = v3(cam->viewport_width, 0, 0);
	cam->vertical = v3(0, cam->viewport_height, 0);
	cam->lower_left_corner = sub(sub(cam->origin, mdiv(cam->horizontal, 2)), mdiv(cam->vertical, 2));
	cam->lower_left_corner = sub(cam->lower_left_corner, v3(0, 0, cam->focal_length));
}

ray get_ray(camera *cam, double u, double v) {
	vec3 dir = add(add(cam->lower_left_corner, scale(cam->horizontal, u)), scale(cam->vertical, v));
	ray r = {.origin = cam->origin, .direction = sub(dir, cam->origin)};
	return r;

}
#endif