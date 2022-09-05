
#include "Spieler.h"

Spieler::Spieler(const char* ModelFile, Vector& position, float size, int lebenspunkte, Bullet* spielerBullet)
	: Model(ModelFile, position, size), pos(position), lebenspunkte(lebenspunkte), spielerBullet(spielerBullet),direction(0), collision(Collision::NONE)
{
}

Spieler::Spieler(const char* ModelFile, int lebenspunkte, const BaseModel& toCopy)
	: Model(ModelFile, toCopy), pos(toCopy.transform().translation()), lebenspunkte(lebenspunkte), direction(0), collision(Collision::NONE), spielerBullet(nullptr)
{
}

Spieler::~Spieler()
{
	delete this->spielerBullet;
}

void Spieler::steuern(int direction)
{
	if (this->collision == Collision::RIGHT && direction == 1) {
		direction = 0;
	}
	else if (this->collision == Collision::LEFT && direction == -1) {
		direction = 0;
	}
	this->direction = direction * 20;
	this->collision = Collision::NONE;
}

void Spieler::update(float dtime)
{
	Matrix aktuelleTransformation = transform();
	Matrix verschiebungsMatrix;
	verschiebungsMatrix.translation(direction * dtime, 0, 0);
	transform(aktuelleTransformation * verschiebungsMatrix);

	this->spielerBullet->update(dtime);
}

void Spieler::collisionBorder(Collision collision)
{
	this->collision = collision;
}

void Spieler::collisionBullet()
{
	this->lebenspunkte -= 1;
}

void Spieler::draw(const BaseCamera& Cam) {
	Model::draw(Cam);
	if (this->spielerBullet->isMoving()) {
		this->spielerBullet->draw(Cam);
	}
}

void Spieler::shoot(bool shotFired) {
	if (shotFired && !this->spielerBullet->isMoving()) {
		this->spielerBullet->show(this->transform().translation() + Vector(0, 1, 0));
		this->spielerBullet->up();
		this->spielerBullet->shadowCaster(true);
	}
}

void Spieler::reset()
{
	this->lebenspunkte = 3;
	this->show(this->pos);
	this->spielerBullet->reset();
}