#include "Gegner.h"

Gegner::Gegner(const char* ModelFile, Vector& position, float size, int lebenspunkte) : Model(ModelFile, position, size), lebenspunkte(lebenspunkte), bullet(nullptr), geschwindigkeit(0)
{

}

void Gegner::update(float dtime, int anzahlGegner)
{
	Matrix aktuelleTransformation = transform();
	Matrix verschiebungsMatrix;
	verschiebungsMatrix.translation(this->geschwindigkeit * dtime, 0, 0);
	transform(aktuelleTransformation * verschiebungsMatrix);
	
	this->geschwindigkeit = *this->linksRechts * 1 / anzahlGegner * 1000;

	if (this->bullet) {
		this->bullet->update(dtime);
	}
}

void Gegner::collisionFeld(int nesw)
{
	if (nesw == EAST) {
		*this->linksRechts = -1;
	}
	else if (nesw == WEST) {
		*this->linksRechts = 1;
	}
}

void Gegner::setPosition(Vector& position)
{

}

void Gegner::setLinksRechts(int* linksRechts)
{
	this->linksRechts = linksRechts;
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