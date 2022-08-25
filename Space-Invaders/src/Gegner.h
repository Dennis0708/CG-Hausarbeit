#pragma once

#include "Model.h"
#include "Bullet.h"
#include "PhongShader.h"
#include "vector.h"

class Gegner : public Model
{
	float geschwindigkeit;
	int* linksRechts;
	float* runter;
	Collision collision;
	Bullet* bullet;
public:
	Gegner(const char* ModelFile, Vector& position, float size);
	void update(float dtime, int anzahlGegner);
	void collisionBorder(Collision collision);
	void collisionBullet();
	void draw(const BaseCamera& Cam);
	void shoot(Bullet* bullet);
	void reset();
	void setLinksRechts(int* linksRechts) { this->linksRechts = linksRechts; };
	void setRunter(float* runter) { this->runter = runter; };
	Bullet* getBullet() { return this->bullet; };
};