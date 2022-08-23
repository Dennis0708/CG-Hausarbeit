#include "Invasion.h"

Invasion::Invasion(list<Gegner*>* gegnerListe)
	: gegnerListe(gegnerListe), direction(0), down(0), bulletQueue(nullptr), bulletsInGame(new list<Bullet*>), timeSinceShoot(0)
{
}

Invasion::~Invasion()
{
	for (Bullet* bullet : *this->bulletsInGame) {
		this->addBullet(bullet);
	}
	delete this->bulletsInGame;
}

void Invasion::start(float height, int anzahlProReihe, Vector& obenLinks)
{
	this->direction = 1;
	float gegnerWidth = this->gegnerListe->front()->boundingBox().size().X;
	int i = 0;
	float abstand = gegnerWidth * 1.5f;
	for (Gegner* gegner : *gegnerListe) {
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
	//cout << "BulleQueueSize: " << this->bulletQueue->size() << endl;
	this->down = 0;
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

void Invasion::addGegner(Gegner* gegner)
{
	this->gegnerListe->push_back(gegner);
}

void Invasion::removeGegner(Gegner* gegner)
{
	gegner->setPosition(Vector(0, 0, 20));
	this->gegnerListe->remove(gegner);
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
			if (gegner->transform().translation().Y < tmp->transform().translation().Y) {
				untereReihe.insert({ gegner->transform().translation().X, gegner });
			}
		}
		catch (const out_of_range& e) {
			untereReihe.insert({ gegner->transform().translation().X, gegner });
		}
	}
	map<float, Gegner*>::iterator iter;
	int random = rand() % untereReihe.size();
	Gegner* gegner;
	int i = 0;
	for (iter = untereReihe.begin(); iter != untereReihe.end(); iter++) {
		if (i == random) {
			gegner = iter->second;
		}
		i++;
	}
	if (!gegner->getBullet()) {
		gegner->shoot(this->bulletQueue->front());
		this->bulletsInGame->push_back(gegner->getBullet());
		this->bulletQueue->pop();
		return true;
	}
	return false;
}

void Invasion::addBullet(Bullet* bullet)
{
	this->bulletQueue->push(bullet);
}

list<Bullet*>* Invasion::getBulletsInGame()
{
	return this->bulletsInGame;
}
