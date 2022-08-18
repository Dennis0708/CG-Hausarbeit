
#include "Spieler.h"

Spieler::Spieler(const char* ModelFile, Vector& position, float size, int lebenspunkte, Bullet* spielerBullet) : Model(ModelFile, position, size), lebenspunkte(lebenspunkte), spielerBullet(spielerBullet)
{

}

void Spieler::steuern(float linksRechts)
{
	this->linksRechts = linksRechts * 1000;
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
}

void Spieler::draw(const BaseCamera& Cam) {
	Model::draw(Cam);
	this->spielerBullet->draw(Cam);
}

void Spieler::shoot() {
	//if (!this->shotFired) {
		this->spielerBullet->teleport(this->transform().translation() + Vector(0, 1, 0), Vector(0,1,0));
		this->shotFired = true;
	//}
}
