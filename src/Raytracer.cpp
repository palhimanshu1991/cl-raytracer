/*
 * File:   Raytracer.cpp
 * Author: jon
 *
 * Created on November 12, 2013, 6:04 PM
 */

#include <thread>
#include <list>
#include <iostream>
#include <algorithm>

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
	this->scene = &scene;

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
	float scale = size / (float)buf.width;

	RaytracerContext context = {
		this,
		&scene,
		0
	};

	for (int y = 0; y < buf.height; ++y) {

		for (int x = 0; x < buf.width; ++x) {
			btVector3 from = fromBase + btVector3(x * scale - size2, -y * scale + size2, 0);
			btVector3 color = trace(from, dir, context);

			buf(x, y) = toColor(color);

//			xAng += xStep;
		}
//		yAng += yStep;
	}
}

#include "ShaderFuncs.h"

class RayHit : public btCollisionWorld::RayResultCallback {
public:
	struct Hit {
		float hitFrac;
		btVector3 normal;
		SceneObject *so;

		btVector3 pos(const btVector3 &from, const btVector3 &to) {
			return from.lerp(to, hitFrac);
		};

		bool operator<(const Hit &other) const {
			return hitFrac < other.hitFrac;
		}
	};

	const btVector3 &from, &to;

	const Raytracer::RaytracerContext &context;

	static vector<Hit> hits;//hits (static to avoid memory overhead)

	RayHit(const Raytracer::RaytracerContext &context, const btVector3 &from, const btVector3 &to) : context(context), from(from), to(to) {
		hits.clear();
	}

	virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace) {
		SceneObject *so = (SceneObject*)rayResult.m_collisionObject->getUserPointer();

		hits.push_back({
			rayResult.m_hitFraction,
			rayResult.m_hitNormalLocal,
			so,
		});

		//make hasHit work
		m_collisionObject = rayResult.m_collisionObject;

		return rayResult.m_hitFraction;
	}

	btVector4 getColor() {
		if (!hits.size())
			return btVector4(0, 0, 0, 0);
		sort(hits.begin(), hits.end());


		//this is kinda like a shader from here. Except you get all the hits in the z stack

		btVector3 eye(context.scene->cameraPos);

		btVector4 ret = btVector4(0, 0, 0, 0);
		for (Hit &hit : hits) {
			btVector4 illum(0, 0, 0, 0);
			btVector3 pos = hit.pos(from, to);

			//the normals Bullet gives are crap, so let's recalcualte it
			btVector3 normal = hit.normal;
//			btVector3 normal = (pos - hit.so->position).normalize();//assuming only spheres


			if (false) {
				//dump normals
				return btVector4(
					normal.x() * .5 + .5,
					normal.y() * .5 + .5,
					normal.z() * .5 + .5,
					1
				);
			}

			for (Light *l : context.scene->lights) {
				double strength = diffuseStrength(l->position, pos, normal) * .5 + .1;
				strength = clamp(strength);
				illum += strength * hit.so->color;
			}

			//bounce!
			btVector3 newDirection = reflect(pos - eye, normal);
			btVector4 reflectColor = context.rt->trace(pos, newDirection, context) * .5;
//			btVector4 reflectColor(0, 0, 0, 0);



			ret = illum + reflectColor;
			break;

//			if (ret.w() >= 1)
//				break;
		}

//		if (ret.w() < 1)
//			ret *= ret.w();//fade remaining alpha to black

		return ret;
	}
};
vector<RayHit::Hit> RayHit::hits;

btVector4 Raytracer::trace(const btVector3 &from, const btVector3 &direction, const RaytracerContext &context) {
	auto getSky = [&] {
		return btVector4(0, 0, 0, 0);
//		return btVector4(direction.x(), direction.y(), direction.z(), 0);
	};

	if (context.depth > 4)
		return btVector4(1, 1, 1, 1);
//		return getSky();

	RaytracerContext newContext(context);
	newContext.depth++;

	btVector3 to = from + direction * 1000;

	RayHit rayHit(newContext, from, to);
	scene->dynamicsWorld->rayTest(from, to, rayHit);

	if (!rayHit.hasHit())
		return getSky();

	return rayHit.getColor();
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
