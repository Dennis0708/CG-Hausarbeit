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
	void collisionFeld(int nesw) override;
	void teleport(Vector& position, Vector& richtung);
	void stop();
	bool isMoving();
};

