#include "Bullet.h"

Bullet::Bullet(const char* ModelFile, Vector& positon, float size)
	: Model(ModelFile, positon, size), upDown(Vector(0, 0, 0))
{
}

void Bullet::update(float dtime) {
	if (this->upDown.Y != 0) {
		Matrix translationMat;
		float geschwindigkeit = 10;
		translationMat.translation(this->upDown * (1/this->size) * geschwindigkeit * dtime);
		transform(transform() * translationMat);
	}
}

void Bullet::collisionBorder(Collision collision)
{
	this->reset();
}

void Bullet::collisionBullet()
{
	this->reset();
}

void Bullet::reset()
{
	this->stop();
	this->hide();
}