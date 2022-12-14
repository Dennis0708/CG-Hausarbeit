#include "GameBar.h"

GameBar::GameBar(list<Model*>* lebensPunkte, const Vector ursprung, float Width, float Height, float Depth)
	: TriangleBoxModel(Width, Height, Depth), ursprung(ursprung), lebensPunkte(lebensPunkte), BoundingBox(Vector(0,0,0),Vector(Width,Height,Depth))
{
	this->show(ursprung);
}

GameBar::GameBar(const BaseModel& toCopy, list<Model*>* lebensPunkte, const Vector ursprung, float Width, float Height, float Depth)
	: TriangleBoxModel(toCopy, Width, Height, Depth), ursprung(ursprung), lebensPunkte(lebensPunkte), BoundingBox(Vector(0, 0, 0), Vector(Width, Height, Depth))
{
	this->show(ursprung);
}

GameBar::~GameBar()
{
	for (Model* m : *this->lebensPunkte) {
		delete m;
	}
	this->lebensPunkte->clear();
}

void GameBar::init()
{
	float abstandX = this->lebensPunkte->front()->boundingBox().size().X * 2;
	float abstandY = this->lebensPunkte->front()->boundingBox().size().Y * 0.5f;
	int i = 1;
	for (Model* leben : *this->lebensPunkte) {
		Vector position;
		position.X = this->ursprung.X - boundingBox().size().X * 0.5f + i * abstandX;
		position.Y = this->ursprung.Y - abstandY;
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

void GameBar::draw(const BaseCamera& Cam)
{
	TriangleBoxModel::draw(Cam);
	for (Model* pModel : *this->lebensPunkte) {
		pModel->draw(Cam);
	}
}
