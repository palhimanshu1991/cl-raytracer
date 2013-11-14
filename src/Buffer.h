/*
 * File:   Buffer.h
 * Author: jon
 *
 * Created on November 12, 2013, 6:05 PM
 */

#ifndef BUFFER_H
#define	BUFFER_H

#include <vector>

#include <SDL/SDL.h>


class Buffer {
public:
	int width, height;
//	std::vector<int32_t> pixels;

	SDL_Surface *sdlSurface;

	Buffer(int width = 1024, int height = 768) : width(width), height(height) {//, pixels(width * height) {
		sdlSurface = SDL_GetVideoSurface();
	}

	int32_t* getBuffer() {
		SDL_LockSurface(sdlSurface);
		//for now, we assume we're going to the screen and that it's up and the same size
		return (int32_t*)sdlSurface->pixels;
	}

	void releaseBuffer() {
		SDL_UnlockSurface(sdlSurface);
	}

//	int &operator()(int x, int y) {
//		return pixels[x + y * width];
//	}
//
//	int operator()(int x, int y) const {
//		return pixels[x + y * width];
//	}

};


#endif	/* BUFFER_H */

