#ifndef CAMERA_H
#define CAMERA_H

#include "mvec3.h"
#include "ray.h"
typedef struct {
	point3 lookfrom;
	point3 lookat;
	mvec3 vup;
	double vfov;
	float aspect_ratio;
	double aperture;
	double focus_dist;
	float viewport_height;
	float viewport_width;
	double len_radius;
	mvec3 u, v, w;

	point3 origin;
	mvec3 horizontal;
	mvec3 vertical;
	mvec3 lower_left_corner;
} camera;

void setup_camera(camera *cam, point3 lookfrom_t, point3 lookat_t, mvec3 vup_t, double vfov_t, double aspect_ratio_t, double aperture_t, double focus_dist_t) {
	cam->lookfrom = lookfrom_t;
	cam->lookat = lookat_t;
	cam->vup = vup_t;
	cam->vfov = vfov_t;
	cam->aspect_ratio = aspect_ratio_t;
	cam->aperture = aperture_t;
	cam->focus_dist = focus_dist_t;
	double theta = degrees_to_radians(cam->vfov);
	double h = tan(theta / 2);
	cam->viewport_height = 2.0 * h;
	cam->viewport_width = cam->aspect_ratio * cam->viewport_height;

	cam->w = unit_vector(sub(cam->lookfrom, cam->lookat));
	cam->u = unit_vector(cross(cam->vup, cam->w));
	cam->v = cross(cam->w, cam->u);

	cam->origin = cam->lookfrom;
	cam->horizontal = scale(cam->u, cam->viewport_width * cam->focus_dist);
	cam->vertical = scale(cam->v, cam->viewport_height * cam->focus_dist);
	cam->lower_left_corner = sub(sub(cam->origin, mdiv(cam->horizontal, 2)), mdiv(cam->vertical, 2));
	cam->lower_left_corner = sub(cam->lower_left_corner, scale(cam->w, cam->focus_dist));
	cam->len_radius = cam->aperture / 2;
}

ray get_ray(camera *cam, double s, double t) {
	mvec3 rd = scale(random_in_unit_disk(), cam->len_radius);
	mvec3 offset = add(scale(cam->u, rd.x), scale(cam->v, rd.y));
	mvec3 dir = add(add(cam->lower_left_corner, scale(cam->horizontal, s)), scale(cam->vertical, t));
	ray r = {.origin = add(cam->origin, offset), .direction = sub(sub(dir, cam->origin), offset)};
	return r;

}
#endif