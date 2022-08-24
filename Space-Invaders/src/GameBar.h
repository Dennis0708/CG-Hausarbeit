#pragma once

#include "TriangleBoxModel.h"
#include <vector>
#include <list>
#include "Model.h"


class GameBar : public TriangleBoxModel
{
	vector<Model*> *lebensPunkte;
public:
	GameBar(list<Model*>* lebensPunkte);
	void removeLife();
};

