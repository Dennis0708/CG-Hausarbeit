#include "CollisionDetector.h"

CollisionDetector::CollisionDetector(AABB* border) : border(border)
{
}

Collision CollisionDetector::borderCollisionGegner(list<Gegner*>* gegnerListe)
{
	Collision collision;
	for (Gegner* gegner : *gegnerListe) {
		collision = this->borderCollision(gegner);
		if (collision != Collision::NONE) {
			return collision;
		}
	}
	return Collision::NONE;
}

Collision CollisionDetector::borderCollision(Model* model)
{
	Vector minHitbox = model->boundingBox().Min + model->transform().translation();
	Vector maxHitbox = model->boundingBox().Max + model->transform().translation();
	if (this->border->Min.X > minHitbox.X) {
		return Collision::LEFT;
	}
	else if (this->border->Min.Y > minHitbox.Y) {
		return Collision::DOWN;
	}
	else if (this->border->Max.X < maxHitbox.X) {
		return Collision::RIGHT;
	}
	else if (this->border->Max.Y < maxHitbox.Y) {
		return Collision::UP;
	}
	return Collision::NONE;
}

Model* CollisionDetector::collision(Model* model, list<Gegner*>* gegnerList)
{
	for (Gegner* gegner : *gegnerList) {
		if (this->collision(model, gegner)) {
			return gegner;
		}
	}
}

bool  CollisionDetector::collision(Model* model1, Model* model2)
{
	Vector minModel1 = model1->boundingBox().Min + model1->transform().translation();
	Vector maxModel1 = model1->boundingBox().Max + model1->transform().translation();
	Vector minModel2 = model2->boundingBox().Min + model2->transform().translation();
	Vector maxModel2 = model2->boundingBox().Max + model2->transform().translation();
	if (maxModel1.Y > minModel2.Y && minModel1.Y < maxModel2.Y) {
		if (maxModel1.X > minModel2.X && minModel1.X < maxModel2.X) {
			model2->collisionBullet(1);
			model1->collisionBullet(1);
			return true;
		}
	}
	return false;

}
