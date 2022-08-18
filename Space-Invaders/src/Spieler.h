#pragma once

#include "Model.h"
#include "Bullet.h"
#include "PhongShader.h"
#include "vector.h"

class Spieler: public Model
{
	int lebenspunkte;
	float linksRechts;
	Bullet* spielerBullet;
	bool shotFired = false;
public:
	Spieler(const char* ModelFile, Vector& position, float size, int lebenspunkte, Bullet * spielerBullet);
	void steuern(float lr);
	void update(float dtime);
	void collisionFeld(int nesw) override;
	void draw(const BaseCamera& Cam);
	void shoot();
};