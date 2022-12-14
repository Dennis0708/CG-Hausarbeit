#include "Gegner.h"

Gegner::Gegner(const char* ModelFile, Vector& position, float size)
	: Model(ModelFile, position, size), bullet(nullptr), geschwindigkeit(0), collision(Collision::NONE), linksRechts(nullptr), runter(nullptr)
{

}

Gegner::Gegner(const char* ModelFile, const BaseModel& toCopy)
	: Model(ModelFile, toCopy), bullet(nullptr), geschwindigkeit(0), collision(Collision::NONE), linksRechts(nullptr), runter(nullptr)
{

}

void Gegner::update(float dtime, int anzahlGegner)
{
	this->geschwindigkeit = (float)*this->linksRechts * (1.f / anzahlGegner) * 2000.0f;
	Matrix aktuelleTransformation = this->transform();
	Matrix verschiebungsMatrix;
	verschiebungsMatrix.translation(this->geschwindigkeit * dtime, *this->runter, 0);
	this->transform(aktuelleTransformation * verschiebungsMatrix);

	if (this->bullet) {
		if (!this->bullet->isMoving()) {
			this->bullet = NULL;
		}
	}
}


void Gegner::draw(const BaseCamera& Cam) {
	Model::draw(Cam);
}

void Gegner::shoot(Bullet* bullet) {
	this->bullet = bullet;
	if (!this->bullet->isMoving()) {
		this->bullet->show(this->transform().translation());
		this->bullet->down();
	}
}

void Gegner::reset()
{
	this->bullet = nullptr;
}