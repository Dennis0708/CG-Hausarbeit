#pragma once

#include "Bullet.h"
#include "Gegner.h"
#include "Spieler.h"
#include <list>
#include "Collision.h"
#include "Model.h"
#include "Gegner.h"

class CollisionDetector
{
	AABB* border;
public:
	CollisionDetector(AABB* border);
	Collision borderCollisionGegner(list<Gegner*> *modelList);
	Collision borderCollision(Model *model);
	Model* collision(Model* bullet, list<Gegner*>* gegnerList);
	bool collision(Model* model1, Model*  model2);
};