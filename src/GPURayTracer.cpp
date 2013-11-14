/*
 * File:   GPURayTracer.cpp
 * Author: jon
 *
 * Created on November 13, 2013, 5:35 PM
 */

#include "GPURayTracer.h"

#include <vector>

using namespace std;


struct RayTracerContext {
	int width, height;
	float cameraPos[4], cameraDir[4];
};



GPURayTracer::GPURayTracer() :
	device(ocl::getDevice(0, 0)),
	rayKernel(&device, "src/raytrace.cl")
{

	int size = 10;
	vector<float> a(size), b(size), c(size);

	for (int i = 0; i < size; ++i) {
		a[i] = i;
		b[i] = i*2;
	}

	auto clA = device.malloc(size * sizeof(float), CL_MEM_READ_ONLY);
	auto clB = device.malloc(size * sizeof(float), CL_MEM_READ_ONLY);
	auto clC = device.malloc(size * sizeof(float), CL_MEM_WRITE_ONLY);

	clA.copyFrom(&a[0]);
	clB.copyFrom(&b[0]);

	rayKernel.setArgs(clA.mem(), clB.mem(), clC.mem());

	rayKernel.run(size, size);

	device.finish();

	clC.copyTo(&c[0]);

	for (int i = 0; i < size; ++i) {
		cout << " Vals: " << a[i] << " + " << b[i] << " = " << c[i] <<endl;
	}


}

GPURayTracer::~GPURayTracer() {
}

void GPURayTracer::render(Scene &scene, Buffer &buf) {

}


