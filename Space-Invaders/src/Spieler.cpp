
#include "Spieler.h"

Spieler::Spieler(const char* ModelFile, Vector& position, float size, int lebenspunkte, Bullet* spielerBullet) : Model(ModelFile, position, size), lebenspunkte(lebenspunkte), spielerBullet(spielerBullet)
{

}

void Spieler::steuern(float linksRechts)
{
	if (this->nesw == EAST && linksRechts < 0) {
		return;
	}
	else if (this ->nesw == WEST && linksRechts > 0) {
		return;
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

	Vector minHitbox = this->boundingBox().Min + this->transform().translation();
	Vector maxHitbox = this->boundingBox().Max + this->transform().translation();
	//cout << minHitbox.toString() << " : " << maxHitbox.toString() << endl;
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
		this->spielerBullet->teleport(this->transform().translation() + Vector(0, 1, 0), Vector(0, 4, 0));
	}
}
