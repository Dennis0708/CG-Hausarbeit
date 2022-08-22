
#include "Spieler.h"

Spieler::Spieler(const char* ModelFile, Vector& position, float size, int lebenspunkte, Bullet* spielerBullet)
	: Model(ModelFile, position, size), lebenspunkte(lebenspunkte), spielerBullet(spielerBullet),direction(0), collision(Collision::NONE)
{

}

void Spieler::steuern(int direction)
{
	if (this->collision == Collision::RIGHT && direction == 1) {
		direction = 0;
	}
	else if (this->collision == Collision::LEFT && direction == -1) {
		direction = 0;
	}
	this->direction = direction * 1000;
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
	//cout << (collision == Collision::RIGHT? "right": "left") << endl;
}

void Spieler::collisionBullet(int schaden)
{
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

Bullet* Spieler::getBullet()
{
	return this->spielerBullet;
}
