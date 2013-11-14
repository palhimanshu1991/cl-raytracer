/*
 * File:   Scene.h
 * Author: jon
 *
 * Created on November 12, 2013, 6:04 PM
 */
#ifndef SCENE_H
#define	SCENE_H

#include <bullet/btBulletDynamicsCommon.h>
#include <vector>
#include <iostream>

#include <CL/cl.h>

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
	std::vector<SceneObject*> objects;
	std::vector<Light*> lights;
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


	typedef float float4[4];

	//this should be the same as the structs defined in the kernel:
	#define PACKED __attribute__ ((aligned(32)))
	typedef struct SceneInfo {
		float4 cameraPos, cameraDir;
		float4 lightPos;//simple for now
		int32_t width, height, numItems;
	} PACKED SceneInfo;

	typedef struct SceneItem {
		float4 color, position;
		float radius;
	} PACKED SceneItem;


	void toStructs(SceneInfo &info, std::vector<SceneItem> &items) {
		auto copy = [] (float4 dest, const btVector3 &src) {
			//can't memcpy m_floats outright, sometimes bullet has btScaler == double...
			//like on my version *facepalm*

			dest[0] = src.x();
			dest[1] = src.y();
			dest[2] = src.z();
			dest[3] = src.w();
		};

		copy(info.cameraPos, cameraPos);
		copy(info.cameraDir, cameraDir);

		//assume we have a light. (CLEANUP)
		copy(info.lightPos, lights[0]->position);

		//copy down objects
		items.clear();
		for (SceneObject *obj : objects) {
			//assume everything is a sphere (CLEANUP)
			Ball &ball = *static_cast<Ball*>(obj);

			SceneItem item;
			item.radius = ball.radius;

			copy(item.color, ball.color);
			copy(item.position, ball.position);
//std::cout << "floats " << ball.position.x() << ", "<< ball.position.y() << ", "<< ball.position.z() << ", " << "\n";
//std::cout <<"first item pos " << ball.position.m_floats[0] << ", " << ball.position.m_floats[1] << ", " << ball.position.m_floats[2] << ", " <<ball.position.m_floats[3] << "\n";
//std::cout <<"first item pos " << item.position[0] << ", " << item.position[1] << ", " << item.position[2] << ", " <<item.position[3] << "\n";

			items.push_back(item);
		}

		info.numItems = items.size();
	}

protected:
	btBroadphaseInterface *broadphase;
	btDefaultCollisionConfiguration *collisionConfiguration;
	btCollisionDispatcher *dispatcher;
	btSequentialImpulseConstraintSolver *solver;

};

#endif	/* SCENE_H */

