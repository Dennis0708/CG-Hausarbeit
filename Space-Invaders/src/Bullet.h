#pragma once

#include "TriangleSphereModel.h"
#include "vector.h"
#include "Model.h"
#define _USE_MATH_DEFINES
#include <math.h>


class Bullet : public Model
{
	int strength;
	Vector upDown;
public:
	Bullet(const char* ModelFile, Vector& positon, float size, int strength);
	void update(float dtime);
	void collisionBorder(Collision collision);
	void collisionBullet(int schaden);
	void setPosition(Vector& position, Vector& richtung);
	void stop();
	void reset();
	bool isMoving();
	int getStrength();
};