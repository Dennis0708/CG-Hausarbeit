#include "Invasion.h"

Invasion::Invasion(list<Gegner*>* gegnerListe) : gegnerListe(gegnerListe), direction(0), down(0), bulletQueue(nullptr) /*vorher(0)*/
{
}


void Invasion::start(float height, int anzahlProReihe, Vector& obenLinks)
{
	//this->aktuelleGegnerQueue = new queue<Gegner*>();
	//this->gameHeight = height;
	this->direction = 1;
	//this->vorher = this->direction;
	float gegnerWidth = this->gegnerListe->front()->boundingBox().size().X;
	int i = 0;
	float abstand = gegnerWidth * 1.5f;
	for (Gegner* gegner : *gegnerListe) {
		//this->aktuelleGegnerQueue->push(gegner);
		Vector position;
		position.X = obenLinks.X + (i % anzahlProReihe) * abstand;
		position.Y = obenLinks.Y + (i / anzahlProReihe) * abstand;
		position.Z = obenLinks.Z;
		gegner->setLinksRechts(&this->direction);
		gegner->setPosition(position);
		gegner->setRunter(&this->down);
		i++;
	}
}

void Invasion::update(float dtime)
{
	//this->anzahlGegner = this->aktuelleGegnerQueue->size();

	//if (this->direction != this->vorher) {
	//	this->down = -this->gameHeight *2.f;
	//}
	//else {
	//	this->down = 0;
	//}
	for (Gegner* gegner : *gegnerListe) {
		gegner->update(dtime, this->gegnerListe->size());
	}
	this->down = 0;
	//this->vorher = this->direction;

}

void Invasion::setBulletQueue(queue<Bullet*>* bulletQueue)
{
	this->bulletQueue = bulletQueue;
}

queue<Bullet*>* Invasion::getBulletQueue() {
	return this->bulletQueue;
}

list<Gegner*>* Invasion::getGegnerListe()
{
	return this->gegnerListe;
}

void Invasion::collisionBorder(Collision collision, float down)
{
	if (collision == Collision::RIGHT) this->direction = -1;
	else if (collision == Collision::LEFT) this->direction = 1;
	this->down = down;
}
