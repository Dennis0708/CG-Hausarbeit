#pragma once

#include "TriangleSphereModel.h"
#include "vector.h"
#include "Model.h"
#define _USE_MATH_DEFINES
#include <math.h>


class Bullet : public Model
{
	Vector upDown;
public:
	Bullet(const char* ModelFile, Vector& positon, float size);
	void update(float dtime);
	void collisionBorder(Collision collision);
	void collisionBullet();
	void up() { this->upDown = Vector(0, 1, 0); };
	void stop() { this->upDown = Vector(0, 0, 0); };
	void down() { this->upDown = Vector(0, -1, 0); };
	void reset();
	bool isMoving() { return this->upDown.Y != 0; };
};