#include "GameBar.h"

GameBar::GameBar(list<Model*> *lebensPunkte):lebensPunkte(nullptr)
{
	this->lebensPunkte = new vector<Model*>();
	for (Model* leben : *lebensPunkte) {
		this->lebensPunkte->push_back(leben);
	}
}

void GameBar::removeLife()
{
	this->lebensPunkte->pop_back();
}
