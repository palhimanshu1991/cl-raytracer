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
#include <boost/timer.hpp>

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
	boost::timer t;

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

	for (int y = 0; y < buf.height; ++y) {

		for (int x = 0; x < buf.width; ++x) {
			btVector3 from = fromBase + btVector3(x * scale - size2, y * scale - size2, 0);
			btVector3 color = trace(from, dir);

			buf(x, y) = toColor(color);

//			xAng += xStep;
		}
//		yAng += yStep;
	}

	cout << "Render took " << t.elapsed() * 1000 << "ms\n";
}


class RayHit : public btCollisionWorld::RayResultCallback {
public:
	struct Hit {
		float hitFrac;
		btVector3 normal;
		SceneObject *so;

		btVector3 pos(const btVector3 &from, const btVector3 &to) {
			return from.lerp(to, hitFrac);
		};

		int operator<(const Hit &other) const {
			return hitFrac > other.hitFrac ? 1 : -1;
		}
	};

	const btVector3 &from, &to;

	const Scene &scene;

	static vector<Hit> hits;//hits (static to avoid memory overhead)

	RayHit(const Scene &scene, const btVector3 &from, const btVector3 &to) : scene(scene), from(from), to(to) {
		hits.clear();
	}

	virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace) {
		SceneObject *so = (SceneObject*)rayResult.m_collisionObject->getUserPointer();

		hits.push_back({
			rayResult.m_hitFraction,
			rayResult.m_hitNormalLocal,
			so,
		});

		return rayResult.m_hitFraction;
	}

	btVector3 getColor() {
		if (!hits.size())
			return btVector3(0, 0, 0);

		sort(hits.begin(), hits.end());


		auto diffuse = [] (btVector3 light, btVector3 pos, btVector3 normal) {
			btVector3 lightNormal = (light - pos).normalize();
			return lightNormal.dot(normal) + .2;
		};

		//this is kinda like a shader from here. Except you get all the hits in the z stack


		btVector4 ret = btVector4(0, 0, 0, 0);
		for (Hit &hit : hits) {
			btVector3 illum(0, 0, 0), pos = hit.pos(from, to);

			for (Light *l : scene.lights) {
				illum += diffuse(l->position, pos, hit.normal) * hit.so->color;
			}

			ret.setValue(illum.x(), illum.y(), illum.z(), 1);
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

btVector3 Raytracer::trace(const btVector3 &from, const btVector3 &direction) {
	btVector3 to = from + direction * 1000;

	RayHit rayHit(*scene, from, to);
	scene->dynamicsWorld->rayTest(from, to, rayHit);

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
