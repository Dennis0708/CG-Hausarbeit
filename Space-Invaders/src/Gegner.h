#pragma once

#include "Model.h"
#include "Bullet.h"
#include "PhongShader.h"
#include "vector.h"

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

class Gegner : public Model
{
	int lebenspunkte;
	int geschwindigkeit;
	int* linksRechts;
	int nesw;
	Bullet* bullet;
public:
	Gegner(const char* ModelFile, Vector& position, float size, int lebenspunkte);
	void update(float dtime, int anzahlGegner);
	void collisionFeld(int nesw) override;
	void setPosition(Vector& position);
	void setLinksRechts(int* linksRechts);
	void draw(const BaseCamera& Cam);
	void shoot(Bullet* bullet);
};