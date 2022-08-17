
#include "Spieler.h"

Spieler::Spieler(const char* ModelFile, bool FitSize, int lebenspunkte) : Model(ModelFile, FitSize), lebenspunkte(lebenspunkte)
{
}

void Spieler::steuern(float linksRechts)
{
	this->linksRechts = linksRechts * 1000;
}

void Spieler::update(float dtime)
{
	Matrix aktuelleTransformation = transform();

	Matrix verschiebungsMatrix;

	verschiebungsMatrix.translation(linksRechts * dtime,0,0);

	transform(aktuelleTransformation * verschiebungsMatrix);
}
