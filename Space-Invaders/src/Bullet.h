#pragma once

#include "TriangleSphereModel.h"
#include "vector.h"


class Bullet : public TriangleSphereModel
{
	int strength;
	Vector upDown;
public:
	Bullet(float Radius, int Stacks, int Slices, int strength, Vector coordinates, bool up);
	void update(float dtime);
	void teleport(Vector coordinates);
	void stop();
};

