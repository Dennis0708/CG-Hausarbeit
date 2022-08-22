#pragma once

#include "Gegner.h"
#include <list>
#include <queue>

class Invasion
{
	list<Gegner*>* gegnerListe;
	float timeSinceShoot;
	int direction;
	float down;
	queue<Bullet*>* bulletQueue;
public:
	Invasion(list<Gegner*>* gegnerListe);
	void init();
	void start(float heigt, int anzahlProReihe, Vector& obenLinks);
	void update(float dtime);
	void setBulletQueue(queue<Bullet*>* bulletQueue);
	queue<Bullet*>* getBulletQueue();
	list<Gegner*>* getGegnerListe();
	void collisionBorder(Collision collision, float down);
	void addGegner(Gegner* gegner);
	void removeGegner(Gegner* gegner);
	bool shoot();
	void addBullet(Bullet* bullet);
};

