/*
 * File:   Raytracer.h
 * Author: jon
 *
 * Created on November 12, 2013, 6:04 PM
 */

#ifndef RAYTRACER_H
#define	RAYTRACER_H

#include "Scene.h"
#include "Buffer.h"

class Raytracer {
public:
	Raytracer() {}

	void render(Scene &scene, Buffer &buf);

protected:
	/* Raytraces from the given point in the given direction. Returns the pixel color. */
	btVector3 trace(const btVector3 &from, const btVector3 &direction);

	Scene *scene = nullptr;

};

#endif	/* RAYTRACER_H */

