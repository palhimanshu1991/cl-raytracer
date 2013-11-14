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
	struct RaytracerContext {
		Raytracer *rt;
		Scene *scene;
		int depth;
	};

	Raytracer(Scene &scene, Buffer &buffer) : scene(scene), buffer(buffer) {}
	virtual ~Raytracer() {}

	/** Snapshots the scene data and starts rendering. */
	virtual void startRender();
	/** Waits for the render to complete. After this returns, the buffer should contain the scene as it was at startRender. */
	virtual void completeRender();

	/* Raytraces from the given point in the given direction. Returns the pixel color. */
	btVector4 trace(const btVector3 &from, const btVector3 &direction, const RaytracerContext &context);

	Scene &scene;
	Buffer &buffer;

};

#endif	/* RAYTRACER_H */

