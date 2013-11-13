
#include <SDL/SDL.h>
#include <list>
#include <iostream>
#include "Scene.h"
#include "Raytracer.h"

using namespace std;

class Screen {
public:
	int width, height;
	SDL_Surface *screen;

	Screen(int width, int height) : width(width), height(height) {
		SDL_Init(SDL_INIT_VIDEO);

		screen = SDL_SetVideoMode(width, width, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
		SDL_WM_SetCaption("Raytracer", 0);
	}

	~Screen() {
		SDL_Quit();
	}

	void display(const Buffer &buf) {
		SDL_Rect rect = {0, 0, 1, 1};

		for (int x = 0; x < buf.width; ++x) {
			for (int y = 0; y < buf.height; ++y) {
				rect.x = x; rect.y = y;
				SDL_FillRect(screen, &rect, buf(x, y));
			}
		}

		SDL_Flip(screen);
	}
};

void setupScene(Scene &scene) {
	scene.add(new Ball(btVector3(0, 0, 0), .5, btVector4(1, 0, 0, 1)));
	scene.add(new Ball(btVector3(1, 1, 0), .6, btVector4(0, 1, 0, 1)));
	scene.add(new Ball(btVector3(.2, -.3, -1), 1, btVector4(0, 0, 1, 1)));
}

int main() {
	int w = 500, h = 500;

	Buffer buf(w, h);
	Scene scene;
	Raytracer tracer;

	setupScene(scene);

	tracer.render(scene, buf);

	Screen screen(w, h);
	screen.display(buf);
	SDL_Delay(3 * 1000);
}