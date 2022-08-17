#pragma once

#include "Model.h"
#include "Bullet.h"
#include "PhongShader.h"

class Spieler: public Model
{
	int lebenspunkte;
	float linksRechts;
	Bullet* spielerBullet;
	bool shotFired = false;
public:
	Spieler(const char* ModelFile, bool FitSize, int lebenspunkte, Bullet * spielerBullet);
	void steuern(float lr);
	void update(float dtime);
	void draw(const BaseCamera& Cam);
	void shoot();
};