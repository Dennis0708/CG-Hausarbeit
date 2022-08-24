#pragma once

#include "TriangleBoxModel.h"
#include <list>

class Barriere
{
	list<TriangleBoxModel*>* barrierePartikel;
	AABB BoundingBox;
public:
	Barriere(list<TriangleBoxModel*>* barrierePartikel);
	~Barriere();
	void init(int anzahlProReihe, Vector& obenLinks);
	void collisionBullet(TriangleBoxModel * partikel);
	void draw(const BaseCamera& Cam);
	list<TriangleBoxModel*>* getPartikel();
	void hide(TriangleBoxModel* partikel);
};