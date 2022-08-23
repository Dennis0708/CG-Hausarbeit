#pragma once

#include "Gegner.h"
#include <list>
#include <queue>

class Invasion
{
	list<Gegner*>* gegnerListe;
	queue<Bullet*>* bulletQueue;
	list<Bullet*>* bulletsInGame;
	float timeSinceShoot;
	int direction;
	float down;
public:
	Invasion(list<Gegner*>* gegnerListe);
	~Invasion();
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
	list<Bullet*> *getBulletsInGame();
};

