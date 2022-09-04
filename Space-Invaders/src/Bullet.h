#pragma once

#include "TriangleSphereModel.h"
#include "vector.h"
#include "Model.h"
#include "Lights.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "LineBoxModel.h"
#include "PhongShader.h"

class Bullet : public Model
{
	Vector upDown;
	PointLight* light;
	float lastDtime = 0;
	LineBoxModel* hitBox;
public:
	Bullet(const char* ModelFile, Vector& positon, float size, PointLight* light = nullptr);
	Bullet(const char* ModelFile, const Model& toCopy, PointLight* light = nullptr);
	virtual ~Bullet() = default;
	void createHitBox();
	void update(float dtime);
	void collisionBorder(Collision collision);
	void collisionBullet();
	void up() {
		this->activateLight();
		this->upDown = Vector(0, 1, 0);
	};
	void stop() {
		this->deactivateLight();
		this->upDown = Vector(0, 0, 0);
	};
	void down() {
		this->activateLight();
		this->upDown = Vector(0, -1, 0);
	};
	void reset();
	bool isMoving() { return this->upDown.Y != 0; };
	void draw(const BaseCamera& Cam) override;
	void activateLight();
	void deactivateLight();

};