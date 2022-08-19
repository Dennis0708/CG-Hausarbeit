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
public:
	Invasion(list<Gegner*>* gegnerListe);
	void init();
	void start(int width, int anzahlProReihe);
	void update(float dtime);
};

