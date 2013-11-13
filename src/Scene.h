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
	virtual ~SceneObject() {
		delete rb;
	}

	btVector4 color = btVector4(1, 0, 0, 1);
	btVector3 position = btVector3(0, 0, 0);

	virtual btRigidBody* getRigidBody() = 0;

	btDefaultMotionState ms;
	btRigidBody *rb = nullptr;
};

class Ball : public SceneObject{
public:
	Ball(btVector3 pos, float radius, btVector4 color) : shape(radius) {
		position = pos;
		this->color = color;

		this->radius = radius;


	}

	float radius;

	btRigidBody* getRigidBody() {
		if (!rb) {
			btRigidBody::btRigidBodyConstructionInfo rbci(
				1,
				&ms,
				&shape,
				btVector3(1, 1, 1)
			);
			rb = new btRigidBody(rbci);
			rb->setWorldTransform(btTransform(btQuaternion::getIdentity(), position));
			rb->setUserPointer(this);
		}

		return rb;
	}

	btSphereShape shape;

};

class Light {
public:
	Light(btVector3 position) : position(position) {}
	btVector3 position;
};

class Scene {
public:
	std::list<SceneObject*> objects;
	std::list<Light*> lights;
	btDiscreteDynamicsWorld *dynamicsWorld;

	btVector3 cameraPos = btVector3(0, 0, 5);
	btVector3 cameraDir = btVector3(0, 0, -1);
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
		objects.push_back(obj);
		dynamicsWorld->addRigidBody(obj->getRigidBody());
	}

	void add(Light *obj) {
		lights.push_back(obj);
	}

	virtual ~Scene() {
		for (SceneObject *b : objects)
			delete b;
		for (Light *l : lights)
			delete l;

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

