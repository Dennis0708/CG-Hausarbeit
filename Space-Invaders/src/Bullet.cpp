#include "Bullet.h"

Bullet::Bullet(const char* ModelFile, Vector& positon, float size, int strength) : Model(ModelFile, positon, size), upDown(Vector(0,0,0)), strength(strength) {
}

void Bullet::update(float dtime) {
	if (this->upDown.Y != 0) {
		//cout << upDown.toString() << endl;
		Matrix translationMat;
		translationMat.translation(this->upDown * dtime);
		transform(transform() * translationMat);
	}
}

void Bullet::collisionFeld(int nesw)
{
}

void Bullet::teleport(Vector& position, Vector& richtung) {
	this->upDown = richtung;
	cout << position.toString() << endl;
	Matrix translationMat;
	translationMat.translation(position);
	this->transform(transform() * translationMat);
}

void Bullet::stop() {
	this->upDown = Vector(0, 0, 0);
}