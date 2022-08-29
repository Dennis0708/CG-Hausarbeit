#pragma once

#include "Model.h"
#include "Bullet.h"
#include "PhongShader.h"
#include "vector.h"
#include "Drawable.h"

class Spieler: public Model, public Drawable
{
	int lebenspunkte;
	int direction;
	Bullet* spielerBullet;
	Collision collision;
public:
	Spieler(const char* ModelFile, Vector& position, float size, int lebenspunkte, Bullet * spielerBullet);
	void steuern(int direction);
	void update(float dtime);
	void collisionBorder(Collision collision);
	void collisionBullet();
	void draw(const BaseCamera& Cam) override;
	void shoot(bool shotFired);
	void reset();
	Bullet* getBullet() { return this->spielerBullet; };
	int getLebenspunkte() { return this->lebenspunkte; };
};