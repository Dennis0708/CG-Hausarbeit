#include "Bullet.h"

Bullet::Bullet(float Radius, int Stacks, int Slices, int strength, Vector coordinates, bool up) : TriangleSphereModel(Radius, Stacks, Slices), strength(strength) {
	this->teleport(coordinates);
	if (up) {
		this->upDown = Vector(0, 10, 0);
	}
	else {
		this->upDown = Vector(0, -10, 0);
	}
}

void Bullet::update(float dtime) {
	Matrix transformMat =  transform();
	Matrix translationMat;
	translationMat.translation(this->upDown * dtime);
	transform(transformMat * translationMat);
}

void Bullet::teleport(Vector coordinates) {
	Matrix translationMat;
	translationMat.translation(coordinates);
	this->transform(translationMat);
}

void Bullet::stop() {
	this->upDown = Vector(0, 0, 0);
}