#include "Invasion.h"

Invasion::Invasion(list<Gegner*>* gegnerListe)
	: gegnerListe(gegnerListe), direction(0), down(0), bulletQueue(NULL),
	bulletsInGame(new list<Bullet*>), timeSinceShoot(0), anzahlProReihe(0)
{
}

Invasion::~Invasion()
{
	for (Bullet* bullet : *this->bulletsInGame) {
		this->addBullet(bullet);
	}
	delete this->bulletsInGame;
}

void Invasion::start(int anzahlProReihe,const  Vector& obenLinks)
{
	this->anzahlProReihe = anzahlProReihe;
	this->obenLinks = obenLinks;
	this->direction = 1;
	float gegnerWidth = this->gegnerListe->front()->boundingBox().size().X;
	float gegnerHeight = this->gegnerListe->front()->boundingBox().size().Y;
	int i = 0;
	float abstand = gegnerWidth * 1.5f;
	for (Gegner* gegner : *gegnerListe) {
		Vector position;
		position.X = obenLinks.X + (i % anzahlProReihe) * abstand;
		position.Y = obenLinks.Y - (i / anzahlProReihe) * abstand - gegnerHeight;
		position.Z = obenLinks.Z;
		gegner->setLinksRechts(&this->direction);
		gegner->show(position);
		gegner->setRunter(&this->down);
		i++;
	}
}

void Invasion::reset(list<Gegner*>* gegnerListe)
{
	delete this->gegnerListe;
	this->gegnerListe = gegnerListe;
	for (Gegner* gegner : *this->gegnerListe) {
		gegner->reset();
	}
	this->start(this->anzahlProReihe, this->obenLinks);
}

void Invasion::update(float dtime)
{
	this->timeSinceShoot += dtime;
	for (Gegner* gegner : *gegnerListe) {
		gegner->update(dtime, this->gegnerListe->size());

	}
	if (this->timeSinceShoot >= 2) {
		if (this->shoot()) {
			this->timeSinceShoot = 0;
		}
	}
	list<Bullet*> toRemove;
	for (Bullet* bullet : *this->bulletsInGame) {
		bullet->update(dtime);
		if (!bullet->isMoving()) {
			this->bulletQueue->push(bullet);
			toRemove.push_back(bullet);
		}
	}
	for (Bullet* bullet : toRemove) {
		this->bulletsInGame->remove(bullet);
	}

	this->down = 0;
}

void Invasion::collisionBorder(Collision collision, float down)
{
	if (collision == Collision::RIGHT) this->direction = -1;
	else if (collision == Collision::LEFT) this->direction = 1;
	this->down = down;
}

bool Invasion::shoot()
{
	if (this->bulletQueue->empty() || this->gegnerListe->empty()) {
		return false;
	}
	map<float, Gegner*> untereReihe;
	for (Gegner* gegner : *gegnerListe) {
		try {
			Gegner* tmp = untereReihe.at(gegner->transform().translation().X);
			if (tmp != gegner) {
				if (gegner->transform().translation().Y < tmp->transform().translation().Y) {
					untereReihe.at(gegner->transform().translation().X) = gegner;
				}
			}
		}
		catch (const out_of_range& e) {
			untereReihe.insert({ gegner->transform().translation().X, gegner });
		}
	}
	map<float, Gegner*>::iterator iter;
	int random = rand() % untereReihe.size();
	Gegner* gegnerToShoot;
	int i = 0;
	for (iter = untereReihe.begin(); iter != untereReihe.end(); iter++) {
		if (i == random) {
			gegnerToShoot = iter->second;
		}
		i++;
	}
	if (!gegnerToShoot->getBullet()) {
		gegnerToShoot->shoot(this->bulletQueue->front());
		this->bulletsInGame->push_back(gegnerToShoot->getBullet());
		this->bulletQueue->pop();
		return true;
	}
	return false;
}

void Invasion::addGegner(Gegner* gegner)
{
	this->gegnerListe->push_back(gegner);
}

void Invasion::removeGegner(Gegner* gegner)
{
	gegner->hide();
	this->gegnerListe->remove(gegner);
}

void Invasion::addBullet(Bullet* bullet)
{
	this->bulletQueue->push(bullet);
}