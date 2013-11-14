/*
 * File:   GPURayTracer.cpp
 * Author: jon
 *
 * Created on November 13, 2013, 5:35 PM
 */

#include "GPURayTracer.h"

#include <vector>

using namespace std;


GPURayTracer::GPURayTracer(Scene &scene, Buffer &buf) :
	Raytracer(scene, buf),
	device(ocl::getDevice(0, 0)),
	rayKernel(&device, "src/raytrace.cl", "", false)
{
	//allocate memory for the scene (we assume the scene object count doesn't change)
	clSceneInfo = device.malloc(sizeof(Scene::SceneInfo), CL_MEM_READ_ONLY);
	clSceneItems = device.malloc(sizeof(Scene::SceneItem) * scene.objects.size(), CL_MEM_READ_ONLY);

	//allocate results buffer
	clResults = device.malloc(sizeof(int32_t) * buf.width * buf.height, CL_MEM_WRITE_ONLY);

}
GPURayTracer::~GPURayTracer() {
}


void GPURayTracer::startRender() {
	//package the scene data
	Scene::SceneInfo sceneInfo;
	vector<Scene::SceneItem> sceneItems;
	scene.toStructs(sceneInfo, sceneItems);

	//also add in the image size
	sceneInfo.width = buffer.width;
	sceneInfo.height = buffer.height;

	//copy the scene data to the GPU
	clSceneInfo.copyFrom(&sceneInfo);
	clSceneItems.copyFrom(&sceneItems[0]);

	//setup arguments
	rayKernel.setArgs(clSceneInfo.mem(), clSceneItems.mem(), clResults.mem());
	rayKernel.setDims(0, 0, buffer.width);
	rayKernel.setDims(1, 0, buffer.height);

	//start kernel
	rayKernel.run();
}

void GPURayTracer::completeRender() {
	//wait for everything to get done
	device.finish();

	//save results
	clResults.copyTo(buffer.getBuffer());
	buffer.releaseBuffer();
}


