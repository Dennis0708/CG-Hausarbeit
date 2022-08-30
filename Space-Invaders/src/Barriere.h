#pragma once

#include "TriangleBoxModel.h"
#include "Drawable.h"
#include <list>

class Barriere: public Drawable
{
	list<TriangleBoxModel*>* barrierePartikel;
	AABB BoundingBox;
	int anzahlProReihe;
	Vector ursprung;
public:
	Barriere(list<TriangleBoxModel*>* barrierePartikel);
	virtual ~Barriere();
	void init(int anzahlProReihe, Vector& obenLinks);
	void reset(list<TriangleBoxModel*>* barrierePartikel);
	void collisionBullet(TriangleBoxModel * partikel);
	void draw(const BaseCamera& Cam) override;
	list<TriangleBoxModel*>* getPartikel();
	void remove(TriangleBoxModel* partikel);
};