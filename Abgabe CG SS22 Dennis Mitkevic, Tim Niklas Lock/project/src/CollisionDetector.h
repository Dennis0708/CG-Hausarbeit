#pragma once

#include "Bullet.h"
#include "Gegner.h"
#include "Spieler.h"
#include <list>
#include "Collision.h"
#include "Model.h"
#include "Gegner.h"
#include "TriangleBoxModel.h"

class CollisionDetector
{
	AABB* border;
public:
	CollisionDetector(AABB* border);
	Collision borderCollisionGegner(list<Gegner*> *modelList);
	Collision borderCollision(Model *model);
	Gegner* collision(BaseModel* bullet, list<Gegner*>* gegnerList);
	list<TriangleBoxModel*> collision(BaseModel* bullet, list<TriangleBoxModel*>* barriere);
	bool collision(BaseModel* model1, BaseModel*  model2);
};

