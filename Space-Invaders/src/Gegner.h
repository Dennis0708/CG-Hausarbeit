#pragma once

#include "Model.h"
#include "Bullet.h"
#include "PhongShader.h"
#include "vector.h"

class Gegner : public Model
{
	int lebenspunkte;
	float geschwindigkeit;
	int* linksRechts;
	float* runter;
	Collision collision;
	Bullet* bullet;
public:
	Gegner(const char* ModelFile, Vector& position, float size, int lebenspunkte);
	void update(float dtime, int anzahlGegner);
	void collisionBorder(Collision collision) override;
	void collisionBullet(int schaden) override;
	void setPosition(Vector& position);
	void setLinksRechts(int* linksRechts);
	void setRunter(float* runter);
	void draw(const BaseCamera& Cam);
	void shoot(Bullet* bullet);
	Bullet* getBullet();
};