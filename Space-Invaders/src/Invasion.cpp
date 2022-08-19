#include "Invasion.h"

Invasion::Invasion(list<Gegner*>* gegnerListe) : gegnerListe(gegnerListe), linksRechts(0)
{
	this->init();
}

void Invasion::init() {
	for (Gegner* gegner : *gegnerListe) {
		this->aktuelleGegnerQueue->push(gegner);

	}
}

void Invasion::start(int width, int anzahlProReihe)
{
	this->linksRechts = 1;
	int i = 0;
	for (Gegner* gegner : *gegnerListe) {
		gegner->setLinksRechts(&this->linksRechts);
		gegner->setPosition(Vector(0,0,0)); //TODO
		i++;
	}
}

void Invasion::update(float dtime)
{
	for (Gegner* gegner : *gegnerListe) {
		gegner->update(dtime, this->anzahlGegner);
	}
}
