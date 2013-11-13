/*
 * File:   Scene.h
 * Author: jon
 *
 * Created on November 12, 2013, 6:04 PM
 */
#ifndef SCENE_H
#define	SCENE_H

#include <bullet/btBulletDynamicsCommon.h>
#include <list>
#include <iostream>

#include "Buffer.h"

class SceneObject {
public:
	virtual ~SceneObject() = 0;

	btVector4 color = btVector4(1, 0, 0, 1);
	btVector3 position = btVector3(0, 0, 0);
};

class Ball {
public:

};

class Scene {
public:
	std::list<SceneObject*> objects;
	btDiscreteDynamicsWorld *dynamicsWorld;

	btVector3 cameraPos = btVector3(0, 0, 5);
	btVector3 cameraDir = btVector3(0, 0, 1);
	float xFov = 45, yFov = 45;

	Scene() {
		// http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Hello_World
		broadphase = new btDbvtBroadphase();
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		solver = new btSequentialImpulseConstraintSolver();
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	}

	void add(SceneObject *obj) {

	}

	virtual ~Scene() {
		for (SceneObject *b : objects)
			delete b;

		delete dynamicsWorld;
		delete solver;
		delete dispatcher;
		delete collisionConfiguration;
		delete broadphase;
	}

protected:
	btBroadphaseInterface *broadphase;
	btDefaultCollisionConfiguration *collisionConfiguration;
	btCollisionDispatcher *dispatcher;
	btSequentialImpulseConstraintSolver *solver;

};

#endif	/* SCENE_H */

