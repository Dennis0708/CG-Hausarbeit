#include "Barriere.h"

Barriere::Barriere(list<TriangleBoxModel*>* barrierePartikel) : barrierePartikel(barrierePartikel), anzahlProReihe(0)
{
}

Barriere::~Barriere()
{
	delete this->barrierePartikel;
}

void Barriere::init(int anzahlProReihe, Vector& ursprung)
{
	this->anzahlProReihe = anzahlProReihe;
	this->ursprung = ursprung;
	float partikelWidth = this->barrierePartikel->front()->boundingBox().size().X;
	float partikelHeight = this->barrierePartikel->front()->boundingBox().size().Y;
	float partikelDepth = this->barrierePartikel->front()->boundingBox().size().Z;
	this->BoundingBox = { Vector(0, 0, 0), Vector(anzahlProReihe * partikelWidth, this->barrierePartikel->size() / anzahlProReihe * partikelHeight, partikelDepth) };
	int i = 0;
	for (TriangleBoxModel* partikel : *this->barrierePartikel) {
		Vector position;
		position.X = ursprung.X + (i % anzahlProReihe) * partikelWidth;
		position.Y = ursprung.Y + (i / anzahlProReihe) * partikelHeight;
		position.Z = ursprung.Z;
		i++;
		position = position - this->BoundingBox.size() * 0.5f;
		Matrix posMat;
		posMat.translation(position);
		partikel->transform(posMat);
		partikel->shadowCaster(true);
	}
}

void Barriere::reset(list<TriangleBoxModel*>* barrierePartikel)
{
	delete this->barrierePartikel;
	this->barrierePartikel = barrierePartikel;
	this->init(this->anzahlProReihe, this->ursprung);
}

void Barriere::collisionBullet(TriangleBoxModel* partikel)
{
	partikel->shadowCaster(false);
	this->remove(partikel);
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

void Barriere::remove(TriangleBoxModel* partikel)
{
	partikel->hide();
	this->barrierePartikel->remove(partikel);
}
