#pragma once

#include "Gegner.h"
#include "Drawable.h"
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
public:
	Invasion(list<Gegner*>* gegnerListe);
	~Invasion();
	void start(int anzahlProReihe, const Vector& obenLinks);
	void reset(list<Gegner*>* gegnerListe);
	void update(float dtime);
	void collisionBorder(Collision collision, float down);
	bool shoot();
	void addGegner(Gegner* gegner);
	void removeGegner(Gegner* gegner);
	void addBullet(Bullet* bullet);
	queue<Bullet*>* getBulletQueue() { return this->bulletQueue; };
	list<Gegner*>* getGegnerListe() { return this->gegnerListe; };
	void setBulletQueue(queue<Bullet*>* bulletQueue) { this->bulletQueue = bulletQueue; };
	list<Bullet*> *getBulletsInGame() { return this->bulletsInGame; };
	void draw(const BaseCamera& Cam) override;
};

