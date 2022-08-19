
#include "Spieler.h"

Spieler::Spieler(const char* ModelFile, Vector& position, float size, int lebenspunkte, Bullet* spielerBullet) : Model(ModelFile, position, size), lebenspunkte(lebenspunkte), spielerBullet(spielerBullet)
{

}

void Spieler::steuern(int linksRechts)
{
	if (this->nesw == EAST && linksRechts == 1) {
		linksRechts = 0;
	}
	else if (this ->nesw == WEST && linksRechts == -1) {
		linksRechts = 0;
	}
	this->linksRechts = linksRechts * 1000;
	
	this->nesw = -1;
}

void Spieler::update(float dtime)
{
	Matrix aktuelleTransformation = transform();
	Matrix verschiebungsMatrix;
	verschiebungsMatrix.translation(linksRechts * dtime, 0, 0);
	transform(aktuelleTransformation * verschiebungsMatrix);

	this->spielerBullet->update(dtime);
}

void Spieler::collisionFeld(int nesw)
{
	this->nesw = nesw;
}

void Spieler::draw(const BaseCamera& Cam) {
	Model::draw(Cam);
	this->spielerBullet->draw(Cam);
}

void Spieler::shoot() {
	if (!this->spielerBullet->isMoving()) {
		this->spielerBullet->setPosition(this->transform().translation() + Vector(0, 1, 0), Vector(0, 1, 0));
	}
}
