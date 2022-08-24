#include "Barriere.h"

Barriere::Barriere(list<TriangleBoxModel*>* barrierePartikel) : barrierePartikel(barrierePartikel)
{
}

Barriere::~Barriere()
{
	delete this->barrierePartikel;
}

void Barriere::init(int anzahlProReihe, Vector& obenLinks)
{
	float partikelWidth = this->barrierePartikel->front()->boundingBox().size().X;
	int i = 0;
	for (TriangleBoxModel* partikel : *this->barrierePartikel) {
		Vector position;
		position.X = obenLinks.X + (i % anzahlProReihe) * partikelWidth;
		position.Y = obenLinks.Y + (i / anzahlProReihe) * partikelWidth;
		position.Z = obenLinks.Z;
		i++;

		Matrix posMat;
		posMat.translation(position);
		partikel->transform(posMat);
	}
}

void Barriere::collisionBullet()
{
}

void Barriere::draw(const BaseCamera& Cam)
{
	for (TriangleBoxModel* partikel : *this->barrierePartikel) {
		partikel->draw(Cam);
	}
}

list<TriangleBoxModel*>* Barriere::getPartikel()
{
	return this->barrierePartikel;
}

void Barriere::removePartikel(TriangleBoxModel* partikel)
{
}
