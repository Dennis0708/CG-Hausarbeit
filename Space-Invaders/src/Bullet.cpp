#include "Bullet.h"

Bullet::Bullet(const char* ModelFile, Vector& positon, float size, int strength) : Model(ModelFile, positon, size), upDown(Vector(0, 0, 0)), strength(strength) {
}

void Bullet::update(float dtime) {
	if (this->upDown.Y != 0) {
		cout << upDown.toString() << endl;
		Matrix translationMat;
		translationMat.translation(this->upDown * dtime);
		transform(transform() * translationMat);
	}
}

void Bullet::collisionFeld(int nesw)
{
	this->teleport(Vector(0, 0, 20), Vector(0, 0, 0));
}

void Bullet::teleport(Vector& position, Vector& richtung) {
	this->upDown = richtung;
	Matrix translationMat, scaleMat, rotatationMat;
	scaleMat.scale(this->size);
	//rotatationMat.rotationX(M_PI_2);
	translationMat.translation(position);
	this->transform(translationMat * scaleMat);
}

void Bullet::stop() {
	this->upDown = Vector(0, 0, 0);
}

bool Bullet::isMoving() {
	return this->upDown.Y != 0;
}