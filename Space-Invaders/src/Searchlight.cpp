#include "Searchlight.h"

Searchlight::Searchlight(const Invasion& invasion, const Vector& rayPos) :invasion(invasion), rayPos(rayPos), timer(0)
{
	this->target = this->invasion.getRandomGegner();
}

void Searchlight::update(float dtime)
{
	this->timer += dtime;
	if (this->timer > 2.5f || target->transform().translation().Z > 5) {
		this->target = this->invasion.getRandomGegner();
		this->timer = 0;
	}
	Vector wert = this->target->transform().translation() - this->rayPos;
	wert = wert * dtime;
	this->rayPos = this->rayPos + wert;
	this->direction((this->rayPos - this->position()).normalize());
}

void Searchlight::reset(const Vector& rayPos)
{
	this->rayPos = rayPos;
	this->timer = 0;
	this->direction((this->rayPos - this->position()).normalize());
}
