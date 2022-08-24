#pragma once

#include "TriangleBoxModel.h"
#include <list>

class Barriere
{
	list<TriangleBoxModel*>* barrierePartikel;
public:
	Barriere(list<TriangleBoxModel*>* barrierePartikel);
	~Barriere();
	void init(int anzahlProReihe, Vector& obenLinks);
	void collisionBullet();
	void draw(const BaseCamera& Cam);
	list<TriangleBoxModel*>* getPartikel();
private:
	void removePartikel(TriangleBoxModel* partikel);
};