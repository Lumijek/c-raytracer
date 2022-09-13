#include "utils/mvec3.h"
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


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gfx/shader.h"

void ErrorCallback(int i,
    const char * err_str) {
    printf("GLFW Error: %s\n", err_str);
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static bool wireframe = false;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
    	if(!wireframe) {
    		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    	}
    	else {
    		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    	}
    	wireframe = !wireframe;
    }
}
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
	mvec3 unit_direction = unit_vector(r->direction);
	double t = 0.5 * (unit_direction.y + 1.0);
	return add(scale(col(1.0, 1.0, 1.0), (1.0 - t)), scale(col(0.5, 0.7, 1.0), t));
}

void random_scene(hittable_list *world) {
	world->current_index = 0;
	material *ground_material = initialize_lambertian(col(0.5, 0.5, 0.5));
	add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){p3(0.0, -1000, 0), 1000, ground_material}});
	for(int a = -11; a < 11; a++) {
		for(int b = -11; b < 11; b++) {
			double choose_mat = random_double();
			point3 center = p3(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if (length(sub(center, p3(4, 0.2, 0)))> 0.9) {
				material *sphere_material;
				if (choose_mat < 0.8) {
					color albedo = mul(random_vec(), random_vec());
					sphere_material = initialize_lambertian(albedo);
					add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){center, 0.2, sphere_material}});
				}
				else if (choose_mat < 0.95){
					color albedo = random_vec_mm(0.5, 0.1);
					double fuzz = random_double_mm(0, 0.5);
					sphere_material = initialize_metal(albedo, fuzz);
					add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){center, 0.2, sphere_material}});
				}
				else {
					sphere_material = initialize_dielectric(1.5);
					add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){center, 0.2, sphere_material}});				
				}
			}
		}
	}
	
	material *material1 = initialize_dielectric(1.5);
	add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){p3(0, 1, 0), 1.0, material1}});

	material *material2 = initialize_lambertian(col(0.4, 0.2, 0.1));
	add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){p3(-4, 1, 0), 1.0, material2}});

	material *material3 = initialize_metal(col(0.7, 0.6, 0.5), 0.0);
	add_obj(world, (hittable) {.type = SPHERE, .s = (sphere){p3(4, 1, 0), 1.0, material3}});

}
int main() {

	// Image
    glfwSetErrorCallback(ErrorCallback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
	const float aspect_ratio = 3.0 / 2.0;
	const int image_width = 800;
	const int image_height = (int) (image_width / aspect_ratio);
	const int samples_per_pixel = 20;
	const int max_depth = 50;
    GLFWwindow * window = glfwCreateWindow(image_width, image_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD!\n");
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	float vertices[] = {
     1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right
     1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // bottom left
    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // top left
	};
    unsigned int indices[] = {
	    0, 1, 3,
        1, 2, 3
	};
	shader *s = malloc(sizeof(shader));
    char *vertexShaderFile = "shaders/shader.vs";
    char *fragmentShaderFile = "shaders/shader.fs";
    create_shader(s, vertexShaderFile, fragmentShaderFile);
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, & VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(1);
    stbi_set_flip_vertically_on_load(true);
    GLubyte *checkImage = malloc(image_width * image_height * 3 * sizeof(GLubyte));
    int cp = 0;
    int c = 0;
    for (int i = 0; i < image_width; i++) {
      for (int j = 0; j < image_height; j++) {
        checkImage[cp++] = c;
        checkImage[cp++] = c;
        checkImage[cp++] = c;
      }
    }
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, checkImage);
    glGenerateMipmap(GL_TEXTURE_2D);
    
	// World
	hittable_list world;
	random_scene(&world);

	// Camera
	camera cam;

	point3 lookfrom = p3(13, 2, 3);
	point3 lookat = p3(0, 0, 0);
	mvec3 vup = v3(0, 1, 0);
	double dist_to_focus = 10.0;
	double aperture = 0.1;
	setup_camera(&cam, lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
	printf("P3\n%d %d\n255\n", image_width, image_height);

	clock_t begin = clock();
	int cpointer;
	for(int j = image_height; j >= 0; j--) {
		fprintf(stderr, "%d iterations remaining!\n", j);
		for(int i = 0; i < image_width; i++) {
			color pixel_color = col(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++) {
				double u = (double) (((double)(i) + random_double()) / (image_width - 1));
				double v = (double) (((double)(j) + random_double()) / (image_height - 1));
				ray r = get_ray(&cam, u, v);
				iadd(&pixel_color, ray_color(&r, &world, max_depth));
			}
  			
			color out = write_color(pixel_color, samples_per_pixel);
			checkImage[cpointer++] = (int)out.x;
			checkImage[cpointer++] = (int)out.y;
			checkImage[cpointer++] = (int)out.z;
		}
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, checkImage);
        useShader(s);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
        glfwPollEvents();
        //getchar();
	}
	clock_t end = clock();
	fprintf(stderr, "Time spent: %f seconds!\n", (double)(end - begin) / CLOCKS_PER_SEC);
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
        glfwPollEvents();
	}
	glfwTerminate();

}
