/*
 * File:   Screen.h
 * Author: jon
 *
 * Created on November 13, 2013, 6:17 PM
 */

#ifndef SCREEN_H
#define	SCREEN_H

#include <SDL/SDL.h>

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

#endif	/* SCREEN_H */

