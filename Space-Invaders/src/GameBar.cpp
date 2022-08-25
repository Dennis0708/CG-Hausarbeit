#include "GameBar.h"

GameBar::GameBar(list<Model*>* lebensPunkte, const Vector ursprung, float Width, float Height, float Depth)
	: LineBoxModel(Width, Height, Depth), ursprung(ursprung), lebensPunkte(lebensPunkte)
{
	this->show(ursprung);
}

void GameBar::init()
{
	float partikelWidth = this->lebensPunkte->front()->boundingBox().size().X;
	int i = 0;
	for (Model* leben : *this->lebensPunkte) {
		Vector position;
		position.X = this->ursprung.X + i * partikelWidth;
		position.Y = this->ursprung.Y;
		position.Z = this->ursprung.Z;
		i++;

		leben->show(position);
	}
}

void GameBar::removeLife()
{
	if (!this->lebensPunkte->empty()) {
		this->lebensPunkte->back()->hide();
		this->lebensPunkte->pop_back();
	}
}

void GameBar::reset(list<Model*>* lebensPunkte)
{
	delete this->lebensPunkte;
	this->lebensPunkte = lebensPunkte;
	this->init();
}