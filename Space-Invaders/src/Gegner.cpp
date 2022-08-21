#include "Gegner.h"

Gegner::Gegner(const char* ModelFile, Vector& position, float size, int lebenspunkte) : Model(ModelFile, position, size), lebenspunkte(lebenspunkte), bullet(nullptr), geschwindigkeit(0)
{

}

void Gegner::update(float dtime, int anzahlGegner)
{
	this->geschwindigkeit = (float)*this->linksRechts * (1.f / anzahlGegner) * 2000.0f;
	Matrix aktuelleTransformation = transform();
	Matrix verschiebungsMatrix;
	verschiebungsMatrix.translation(this->geschwindigkeit * dtime, *this->runter, 0);
	transform(aktuelleTransformation * verschiebungsMatrix);


	if (this->bullet) {
		this->bullet->update(dtime);
	}
}

void Gegner::collisionBorder(Collision collision)
{
	if (collision == Collision::RIGHT) {
		*this->linksRechts = -1;
	}
	else if (collision == Collision::LEFT) {
		*this->linksRechts = 1;
	}
	*this->runter = true;
}

void Gegner::collisionBullet(int schaden)
{
}

void Gegner::setPosition(Vector& position)
{
	Matrix scaleMat, posMat;
	scaleMat.scale(size);
	posMat.translation(position);

	transform(posMat * scaleMat);
}

void Gegner::setLinksRechts(int* linksRechts)
{
	this->linksRechts = linksRechts;
}

void Gegner::setRunter(float* runter)
{
	this->runter = runter;
}

void Gegner::draw(const BaseCamera& Cam) {
	Model::draw(Cam);
	if (this->bullet) {
		this->bullet->draw(Cam);
	}
}

void Gegner::shoot(Bullet* bullet) {
	this->bullet = bullet;
}