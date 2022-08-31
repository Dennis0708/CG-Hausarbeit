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

void Invasion::start(int anzahlProReihe, const  Vector& obenLinks)
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
	//this->calcBoundingBox();
}

void Invasion::reset(list<Gegner*>* gegnerListe)
{
	delete this->gegnerListe;
	this->gegnerListe = gegnerListe;
	for (Gegner* gegner : *this->gegnerListe) {
		gegner->shadowCaster(true);
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
	if (this->timeSinceShoot >= 0.5f) {
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

	//this->calcBoundingBox();

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
	Gegner* gegnerToShoot = untereReihe.begin()->second;
	int i = 0;
	for (iter = untereReihe.begin(); iter != untereReihe.end(); iter++) {
		if (i == random) {
			gegnerToShoot = iter->second;
		}
		i++;
	}
	if (gegnerToShoot->getBullet() == nullptr) {
		this->bulletQueue->front()->shadowCaster(true);
		gegnerToShoot->shoot(this->bulletQueue->front());
		this->bulletsInGame->push_back(gegnerToShoot->getBullet());
		this->bulletQueue->pop();
		return true;
	}
	return false;
}

Vector Invasion::calcCenter() {
	Vector center = { 0,0,0 };
	for (Gegner* gegner : *this->gegnerListe) {
		center = center + gegner->transform().translation();
	}
	return center * (1.0f / (float)this->gegnerListe->size());
}

void Invasion::calcBoundingBox()
{
	BaseModel* FirstModel = *this->gegnerListe->begin();
	this->bBox = FirstModel->boundingBox().transform(FirstModel->transform());

	for (BaseModel* pModel : *this->gegnerListe)
	{
		AABB Box = pModel->boundingBox().transform(pModel->transform());
		this->bBox.merge(Box);
	}
}

void Invasion::addGegner(Gegner* gegner)
{
	gegner->shadowCaster(true);
	this->gegnerListe->push_back(gegner);
}

void Invasion::removeGegner(Gegner* gegner)
{
	gegner->shadowCaster(false);
	gegner->hide();
	this->gegnerListe->remove(gegner);
}

void Invasion::addBullet(Bullet* bullet)
{
	bullet->shadowCaster(false);
	this->bulletQueue->push(bullet);
}

void Invasion::draw(const BaseCamera& Cam) {
	for (Gegner* gegner : *this->gegnerListe) {
		gegner->draw(Cam);
	}
	for (Bullet* bullet : *this->bulletsInGame) {
		bullet->draw(Cam);
	}
}

const Gegner* Invasion::getRandomGegner() const
{
	int random = rand() % this->gegnerListe->size();
	int i = 0;
	for (Gegner* gegner : *this->gegnerListe) {
		if (i == random) {
			return gegner;
		}
		i++;
	}
	return this->gegnerListe->front();
}
