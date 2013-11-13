/*
 * File:   Buffer.h
 * Author: jon
 *
 * Created on November 12, 2013, 6:05 PM
 */

#ifndef BUFFER_H
#define	BUFFER_H

#include <vector>

class Buffer {
public:
	int width, height;
	std::vector<int> pixels;

	Buffer(int width = 1024, int height = 768) : width(width), height(height), pixels(width * height) {
	}

	int &operator()(int x, int y) {
		return pixels[x + y * width];
	}

	int operator()(int x, int y) const {
		return pixels[x + y * width];
	}

};


#endif	/* BUFFER_H */

