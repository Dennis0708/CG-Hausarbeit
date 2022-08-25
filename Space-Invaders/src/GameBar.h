#pragma once

#include "TriangleBoxModel.h"
#include "LineBoxModel.h"
#include <vector>
#include <list>
#include "Model.h"


class GameBar : public LineBoxModel
{
	list<Model*>* lebensPunkte;
	Vector ursprung;
	AABB BoundingBox;
public:
	GameBar(list<Model*>* lebensPunkte, const Vector ursprung, float Width = 1, float Height = 1, float Depth = 1);
	void init();
	void removeLife();
	void reset(list<Model*>* lebensPunkte);
	const AABB& boundingBox() const { return BoundingBox; }
};

