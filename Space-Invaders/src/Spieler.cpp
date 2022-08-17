
#include "Spieler.h"

Spieler::Spieler(const char* ModelFile, bool FitSize, int lebenspunkte, Bullet* spielerBullet) : Model(ModelFile, FitSize), lebenspunkte(lebenspunkte), spielerBullet(spielerBullet)
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

void Spieler::draw(const BaseCamera& Cam) {
	Model::draw(Cam);
	this->spielerBullet->draw(Cam);
}

void Spieler::shoot() {
	if (!this->shotFired) {
		this->spielerBullet->teleport(this->transform().translation() + Vector(0, 0.5f, 0));
		this->shotFired = true;
	}
}
