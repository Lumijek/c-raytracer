#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>
#include <math.h>
#include "mvec3.h"

static inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}
color write_color(color pixel_color, int samples_per_pixel){

	double r = pixel_color.x;
	double g = pixel_color.y;
	double b = pixel_color.z;

	double scale = (1.0 / samples_per_pixel);
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);
	printf("%d %d %d\n", (int) (256 * clamp(r, 0.0, 0.999)), (int) (256 * clamp(g, 0.0, 0.999)), (int) (256 * clamp(b, 0.0, 0.999)));

	return (color) {(int) (256 * clamp(r, 0.0, 0.999)), (int) (256 * clamp(g, 0.0, 0.999)), (int) (256 * clamp(b, 0.0, 0.999))};
}

#endif