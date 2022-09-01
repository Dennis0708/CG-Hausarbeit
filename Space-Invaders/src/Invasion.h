#pragma once

#include "Gegner.h"
#include "Drawable.h"
#include "Aabb.h"
#include <list>
#include <queue>

class Invasion: public Drawable
{
	list<Gegner*>* gegnerListe;
	queue<Bullet*>* bulletQueue;
	list<Bullet*>* bulletsInGame;
	float timeSinceShoot;
	int direction;
	float down;
	int anzahlProReihe;
	Vector obenLinks;
	AABB bBox;
public:
	Invasion(list<Gegner*>* gegnerListe);
	virtual ~Invasion();
	void start(int anzahlProReihe, const Vector& obenLinks);
	void reset(list<Gegner*>* gegnerListe);
	void update(float dtime);
	void collisionBorder(Collision collision, float down);
	bool shoot();
	Vector calcCenter();
	void calcBoundingBox();
	void addGegner(Gegner* gegner);
	void removeGegner(Gegner* gegner);
	void addBullet(Bullet* bullet);
	const queue<Bullet*>* getBulletQueue() { return this->bulletQueue; }
	list<Gegner*>* getGegnerListe() { return this->gegnerListe; }
	AABB boundingBox() { return this->bBox; }
	void setBulletQueue(queue<Bullet*>* bulletQueue) { this->bulletQueue = bulletQueue; }
	list<Bullet*> *getBulletsInGame() { return this->bulletsInGame; }
	void draw(const BaseCamera& Cam) override;
	const Gegner *getRandomGegner() const;
};

