#pragma once

#include "Lights.h"
#include "Invasion.h"

class Searchlight : public SpotLight
{
	const Invasion& invasion;
	const Gegner* target;
	float timer;
	Vector rayPos;
public:
	Searchlight(const Invasion & invasion, const Vector& rayPos);
	~Searchlight() = default;
	void update(float dtime);
};

