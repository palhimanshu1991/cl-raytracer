/*
 * File:   Raytracer.cpp
 * Author: jon
 *
 * Created on November 12, 2013, 6:04 PM
 */

#include <thread>
#include <list>
#include <iostream>
#include <bits/stl_list.h>
#include <bullet/LinearMath/btVector3.h>

#include "Raytracer.h"
#include "Buffer.h"
#include "Scene.h"

using namespace std;

inline int toColor(const btVector3 &color) {
	int r = color.x() * 255;
	int g = color.y() * 255;
	int b = color.z() * 255;
	return (r << 16) | (g << 8) | (b);
}

void Raytracer::render(Scene &scene, Buffer &buf) {

//	float yStep = scene.yFov / buf.height, yAng = -scene.yFov / 2.0;
//	float xStep = scene.xFov / buf.width, xAng = -scene.xFov / 2.0;
//
//
//	btVector3 xStart = scene.cameraDir.rotate(btVector3(0, 1, 0), -scene.xFov / 2.0);
//	btVector3 xEnd = scene.cameraDir.rotate(btVector3(0, 1, 0), scene.xFov / 2.0);
//
//	btVector3 yStart = scene.cameraDir.rotate(btVector3(1, 0, 0), -scene.yFov / 2.0);
//	btVector3 yEnd = scene.cameraDir.rotate(btVector3(1, 0, 0), scene.yFov / 2.0);

	btVector3 fromBase = scene.cameraPos, dir = scene.cameraDir;
	float size = 5, size2 = size / 2.0;
	float scale = buf.width / size;

	for (int y = 0; y < buf.height; ++y) {

		for (int x = 0; x < buf.width; ++x) {
			btVector3 from = fromBase + btVector3(x * scale - size2, y * scale - size2, 0);
			btVector3 color = trace(from, dir);

			buf(x, y) = toColor(color);

//			xAng += xStep;
		}
//		yAng += yStep;
	}
}

btVector3 Raytracer::trace(const btVector3 &from, const btVector3 &direction) {
	return btVector3(1, .5, .5);

}




//	int threadNum = 20;
//	list<thread> threads;
//
//	for (int t = 0 ; t < threadNum; ++t) {
//		thread t([&] {
//
//		});
//		t.start();
//		threads.push_back(t);
//
//	}
//
//	for (thread &t : threads) {
//		t.join();
//	}
