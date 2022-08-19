#pragma once

#include "Gegner.h"
#include <list>
#include <queue>

class Invasion
{
	list<Gegner*>* gegnerListe;
	queue<Gegner*>* aktuelleGegnerQueue;
	int anzahlGegner;
	int linksRechts;
	float runter;
	float vorher;
	float gameHeight;
	queue<Bullet*>* bulletQueue;
public:
	Invasion(list<Gegner*>* gegnerListe);
	void init();
	void start(float heigt, int anzahlProReihe, Vector& obenLinks);
	void update(float dtime);
	void setBulletQueue(queue<Bullet*>* bulletQueue);
	queue<Bullet*>* getBulletQueue();
};

