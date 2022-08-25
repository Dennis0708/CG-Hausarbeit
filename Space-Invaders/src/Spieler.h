#pragma once

#include "Model.h"
#include "Bullet.h"
#include "PhongShader.h"
#include "vector.h"

class Spieler: public Model
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
	void collisionBullet(int schaden);
	void draw(const BaseCamera& Cam);
	void shoot();
	void reset();
	Bullet* getBullet();
	int getLebenspunkte();
};