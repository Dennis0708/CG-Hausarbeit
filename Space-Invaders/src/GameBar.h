#pragma once

#include "TriangleBoxModel.h"
#include "LineBoxModel.h"
#include <vector>
#include <list>
#include "Model.h"
#include "Drawable.h"

class GameBar : public TriangleBoxModel, public Drawable
{
	list<Model*>* lebensPunkte;
	Vector ursprung;
	AABB BoundingBox;
public:
	GameBar(list<Model*>* lebensPunkte, const Vector ursprung, float Width = 1, float Height = 1, float Depth = 1);
	GameBar(const BaseModel & toCopy, list<Model*>* lebensPunkte, const Vector ursprung, float Width = 1, float Height = 1, float Depth = 1);
	virtual ~GameBar();
	void init();
	void removeLife();
	void reset(list<Model*>* lebensPunkte);
	void draw(const BaseCamera& Cam) override;
	const AABB& boundingBox() const { return BoundingBox; }
};

