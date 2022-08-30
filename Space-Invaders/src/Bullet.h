#pragma once

#include "TriangleSphereModel.h"
#include "vector.h"
#include "Model.h"
#include "Lights.h"
#define _USE_MATH_DEFINES
#include <math.h>


class Bullet : public Model
{
	Vector upDown;
	PointLight* light;
public:
	Bullet(const char* ModelFile, Vector& positon, float size, PointLight* light = nullptr);
	virtual ~Bullet() = default;
	void update(float dtime);
	void collisionBorder(Collision collision);
	void collisionBullet();
	void up() {
		this->activateLight();
		this->upDown = Vector(0, 1, 0);
	};
	void stop() {
		this->deactivateLight();
		this->upDown = Vector(0, 0, 0);
	};
	void down() {
		this->activateLight();
		this->upDown = Vector(0, -1, 0);
	};
	void reset();
	bool isMoving() { return this->upDown.Y != 0; };
	void activateLight();
	void deactivateLight();

};