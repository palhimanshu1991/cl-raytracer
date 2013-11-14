
#include <list>
#include <iostream>

#include "Scene.h"
#include "Screen.h"
#include "Raytracer.h"
#include "GPURayTracer.h"

using namespace std;



void setupScene(Scene &scene) {
	scene.add(new Ball(btVector3(0, 0, 0), .5, btVector4(1, 1, .5, 1)));
	scene.add(new Ball(btVector3(0, 0, 1), .4, btVector4(1, 1, 1, 1)));
	scene.add(new Ball(btVector3(0, 0, 2), .3, btVector4(1, .5, 1, 1)));

	scene.add(new Ball(btVector3(-3, 2, 0), 2, btVector4(0, 1, 0, 1)));
	scene.add(new Ball(btVector3(3, 2, 0), 2, btVector4(0, 0, 1, 1)));

	scene.add(new Ball(btVector3(0, 0, -10), 10, btVector4(1, 1, 1, 1)));

	scene.add(new Light(btVector3(-3, -3, 20)));
}

int main() {
	int w = 500, h = 500;

	Buffer buf(w, h);
	Scene scene;
	GPURayTracer tracer;
//	Raytracer tracer;

	setupScene(scene);

	tracer.render(scene, buf);

	Screen screen(w, h);
	screen.display(buf);

	SDL_Event ev;
	while (SDL_WaitEvent(&ev)) {
		if (ev.type == SDL_QUIT)
			break;
	}
}