#include <iostream>
#include <stdio.h>
#include <math.h>
#include <ostream>
#include <sstream>
#include <float.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "camera.h"
#include "hitable_list.h"
#include "random.h"
#include "material.h"
#include "FileReader.h"


using namespace std;

vec3 color(const ray& r, hitable* world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}


hitable* my_scene() {

	int n = 500;
	hitable** list = new hitable * [n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;

	/*new lambertian(vec3(random_double() * random_double(),
							random_double() * random_double(),
							random_double() * random_double()))*/


	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}


int main()
{
	int nx = 100;
	int ny = 100;
	int ns = 1;

	ifstream file;
	file.open("SceneConfig.txt");

	list<string> text;
	if (file.is_open())
	{
		string line;

		while (getline(file, line))
		{
			stringstream ss(line);

			while (getline(ss, line, ' '))
			{
				text.push_back(line);
			}
		}
	}

	file.close();

	vec3 lookfrom;
	
	hitable** objects = new hitable * [1];
	//objects[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));

	list<string>::iterator it = text.begin();
	for (list<string>::iterator it = text.begin(); it != text.end(); it++)
	{
		string token = *it;
		if (*it == "cameraPos")
		{
			advance(it, 1);
			for (int i = 0; i < 3; i++)
			{
				lookfrom.e[i] = stof(*it);
				advance(it, 1);
			}

		}
		 if (*it == "sphere")
		{
			advance(it, 1);
			int size = stoi(*it);

			//objects = new hitable * [size];
			advance(it, 1);
			sphere *s = new sphere();


			for (int i = 0; i < 3; i++)
			{
				s->center.e[i] = stof(*it);
				advance(it, 1);
			}

			advance(it, 1);
			s->radius = stof(*it);

			advance(it, 1);
			s->mat_ptr = new metal(vec3(0.7, 0.6, 0.5), 0.0);

			//objects[0] = s;
			objects[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));

		}
		 if (*it == "img")
		{

		}
	}


	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;


	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus);
	hitable* world = new hitable_list(objects,1);


	//hitable* list = new hitable[5];

#define CHANNEL_NUM 3

	/*** NOTICE!! You have to use uint8_t array to pass in stb function  ***/
	// Because the size of color is normally 255, 8bit.
	// If you don't use this one, you will get a weird imge.
	uint8_t* pixels = new uint8_t[nx * ny * CHANNEL_NUM];

	int index = 0;
	for (int j = nx - 1; j >= 0; --j)
	{
		for (int i = 0; i < ny; ++i)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random_double()) / float(nx);
				float v = float(j + random_double()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			pixels[index++] = ir;
			pixels[index++] = ig;
			pixels[index++] = ib;
		}
	}

	// if CHANNEL_NUM is 4, you can use alpha channel in png
	stbi_write_png("stbpng.png", nx, ny, CHANNEL_NUM, pixels, nx * CHANNEL_NUM);

	delete[] pixels;

	return 0;
}