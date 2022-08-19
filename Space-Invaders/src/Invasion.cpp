#include "Invasion.h"

Invasion::Invasion(list<Gegner*>* gegnerListe) : gegnerListe(gegnerListe), linksRechts(0), runter(false), vorher(0)
{
}


void Invasion::start(float height, int anzahlProReihe, Vector & obenLinks)
{
	this->aktuelleGegnerQueue = new queue<Gegner*>();
	this->gameHeight = height;
	this->linksRechts = 1;
	this->vorher = this->linksRechts;
	float gegnerWidth = this->gegnerListe->front()->boundingBox().size().X;
	int i = 0;
	float abstand = gegnerWidth * 1.5f;
	for (Gegner* gegner : *gegnerListe) {
		this->aktuelleGegnerQueue->push(gegner);
		Vector position;
		position.X =  obenLinks.X + (i % anzahlProReihe) * abstand;
		position.Y = obenLinks.Y + (i / anzahlProReihe) * abstand;
		position.Z = obenLinks.Z;
		gegner->setLinksRechts(&this->linksRechts);
		gegner->setPosition(position); 
		gegner->setRunter(&this->runter);
		i++;
	}
}

void Invasion::update(float dtime)
{
	this->anzahlGegner = this->aktuelleGegnerQueue->size();
	if (this->linksRechts != this->vorher) {
		this->runter = -this->gameHeight *2.f;
	}
	else {
		this->runter = 0;
	}
	for (Gegner* gegner : *gegnerListe) {
		gegner->update(dtime, this->anzahlGegner);
	}
	this->vorher = this->linksRechts;

}

void Invasion::setBulletQueue(queue<Bullet*>* bulletQueue)
{
	this->bulletQueue = bulletQueue;
}

queue<Bullet*>* Invasion::getBulletQueue() {
	return this->bulletQueue;
}