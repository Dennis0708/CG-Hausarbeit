#include "Bullet.h"

Bullet::Bullet(const char* ModelFile, Vector& positon, float size, PointLight* light)
	: Model(ModelFile, positon, size), upDown(Vector(0, 0, 0)), light(light)
{
	this->hitBox = new LineBoxModel(this->boundingBox().size().X, this->boundingBox().size().Y, this->boundingBox().size().Z);
	PhongShader* pShader = new PhongShader();
	pShader->ambientColor(Color(1,0,0));
	this->hitBox->shader(pShader, true);
	this->hitBox->show(this->transform().translation());
}


void Bullet::update(float dtime) {
	if (this->upDown.Y != 0) {
		Matrix translationMat;
		float geschwindigkeit = 5;
		translationMat.translation(this->upDown * (1 / this->size) * geschwindigkeit * dtime);
		transform(transform() * translationMat);
		if (this->light) {
			this->light->position(this->transform().translation());
		}
		this->lastDtime = dtime;
		this->hitBox->transform(this->transform());
	}
}

void Bullet::collisionBorder(Collision collision)
{
	this->reset();
}

void Bullet::collisionBullet()
{
	this->reset();
}

void Bullet::reset()
{
	this->shadowCaster(false);
	this->stop();
	this->hide();
}

void Bullet::draw(const BaseCamera& Cam)
{
	Model::draw(Cam);
	this->hitBox->draw(Cam);
}

void Bullet::activateLight() {
	if (this->light) {
		this->light->color({ 0,1,0 });
	}

}

void Bullet::deactivateLight() {
	if (this->light) {
		this->light->color({ 0,0,0 });
		
	}

}
