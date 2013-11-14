/*
 * File:   GPURayTracer.h
 * Author: jon
 *
 * Created on November 13, 2013, 5:35 PM
 */

#ifndef GPURAYTRACER_H
#define	GPURAYTRACER_H

#include "ocl.h"

#include "Raytracer.h"

class GPURayTracer : public Raytracer {
public:
	GPURayTracer();
	void render(Scene &scene, Buffer &buf);
	virtual ~GPURayTracer() {}

protected:
	ocl_device device;
	ocl_kernel rayKernel;

	struct RayTracerContext {
		int width, height;
		float cameraPos[4], cameraDir[4];
	};

};

#endif	/* GPURAYTRACER_H */

