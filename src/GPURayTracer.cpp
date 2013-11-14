/*
 * File:   GPURayTracer.cpp
 * Author: jon
 *
 * Created on November 13, 2013, 5:35 PM
 */

#include "GPURayTracer.h"

#include <vector>

using namespace std;


GPURayTracer::GPURayTracer() :
	device(ocl::getDevice(0, 0)),
	rayKernel(&device, "src/raytrace.cl", "", false)
{

}

void GPURayTracer::render(Scene &scene, Buffer &buf) {
	//package the scene data
	Scene::SceneInfo sceneInfo;
	vector<Scene::SceneItem> sceneItems;
	scene.toStructs(sceneInfo, sceneItems);

	//also add in the image size
	sceneInfo.width = buf.width;
	sceneInfo.height = buf.height;

	//allocate and copy the scene data to the GPU
	auto clSceneInfo = device.malloc(sizeof(Scene::SceneInfo), CL_MEM_READ_ONLY);
	clSceneInfo.copyFrom(&sceneInfo);
	auto clSceneItems = device.malloc(sizeof(Scene::SceneItem) * sceneItems.size(), CL_MEM_READ_ONLY);
	clSceneItems.copyFrom(&sceneItems[0]);

	//allocate results buffer
	auto clResults = device.malloc(sizeof(int32_t) * buf.width * buf.height, CL_MEM_WRITE_ONLY);

	//setup arguments
	rayKernel.setArgs(clSceneInfo.mem(), clSceneItems.mem(), clResults.mem());
	rayKernel.setDims(0, 0, buf.width);
	rayKernel.setDims(1, 0, buf.height);

	//start kernel
	rayKernel.run();

	//wait
	device.finish();

	//save results
	clResults.copyTo(&buf.pixels[0]);

	//cout << "first pixel " << buf.pixels[0]<<endl;
}


