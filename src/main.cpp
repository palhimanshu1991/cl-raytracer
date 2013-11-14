
#include <list>
#include <iostream>

#include <boost/timer.hpp>


#include "Scene.h"
#include "Screen.h"
#include "Raytracer.h"
#include "GPURayTracer.h"

using namespace std;



void setupScene(Scene &scene) {
	//center set
	scene.add(new Ball(btVector3(0, 0, 0), .5, btVector4(1, 1, .5, 1)));
	scene.add(new Ball(btVector3(0, 0, 1), .4, btVector4(1, 1, 1, 1)));
	scene.add(new Ball(btVector3(0, 0, 2), .3, btVector4(1, .5, 1, 1)));

	//leftright
	scene.add(new Ball(btVector3(-3, 2, 0), 2, btVector4(0, 1, 0, 1)));
	scene.add(new Ball(btVector3(3, 2, 0), 2, btVector4(0, 0, 1, 1)));

	//big in the back
	scene.add(new Ball(btVector3(0, 0, -10), 10, btVector4(1, 1, 1, 1)));

	//things that fall on things
	scene.add(new Ball(btVector3(.1, 10, .04), .5, btVector4(1, 1, .5, 1)));
	scene.add(new Ball(btVector3(-2.5, 2, 10), .3, btVector4(1, .5, .5, 1)));

	scene.add(new Light(btVector3(-3, -3, 20)));
}

int main() {

	Scene scene;
	setupScene(scene);

	int w = 1440, h = 1440;
//	int w = 500, h = 500;
	Buffer buf(w, h);
	Screen screen(w, h);


	GPURayTracer tracer(scene, buf);
//	Raytracer tracer(scene, buf);


	boost::timer frameTime;
	float timeDelta = 0;
	int numFrames = 0;
	bool running = true, frozen = false;
	while (running) {
		tracer.startRender();
		if (!frozen)
			scene.stepSimulation(timeDelta);
		tracer.completeRender();

		{
			boost::timer g;
			screen.display(buf);
			cout << "Display took " << g.elapsed() * 1000 << "ms\n";
		}

//		if (numFrames & 150)
		timeDelta = frameTime.elapsed();
		frameTime.restart();
		cout << "Render took " << timeDelta * 1000 << "ms\n";

		//read + handle events
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT)
				running = false;
			else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)
				running = false;
			else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_SPACE)
				frozen = !frozen;
		}
	}
}