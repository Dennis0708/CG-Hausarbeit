#pragma once

#include "TriangleBoxModel.h"
#include <list>

class Barriere
{
	list<TriangleBoxModel*>* barrierePartikel;
	AABB BoundingBox;
	int anzahlProReihe;
	Vector ursprung;
public:
	Barriere(list<TriangleBoxModel*>* barrierePartikel);
	~Barriere();
	void init(int anzahlProReihe, Vector& obenLinks);
	void reset(list<TriangleBoxModel*>* barrierePartikel);
	void collisionBullet(TriangleBoxModel * partikel);
	void draw(const BaseCamera& Cam);
	list<TriangleBoxModel*>* getPartikel();
	void remove(TriangleBoxModel* partikel);
};