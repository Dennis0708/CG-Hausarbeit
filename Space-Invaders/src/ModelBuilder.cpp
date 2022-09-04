#include "ModelBuilder.h"

//Model* ModelBuilder::getModel()
//{
//	if (this->model) {
//		return this->model;
//	}
//	else {
//		this->model = new Model();
//	}
//}

ModelBuilder& ModelBuilder::getInstance()
{
	static ModelBuilder instance;
	return instance;
}

//ModelBuilder& ModelBuilder::loadModel(const char* ModelFile, float size)
//{
//	bool ret = this->model.load(ModelFile, size);
//	if (!ret)
//		throw std::exception();
//	//return ModelBuilder::builder;
//
//}

ModelBuilder& ModelBuilder::position(const Vector& pos)
{
	this->model.show(pos);
	//return ModelBuilder::builder;
	return this->getInstance();
}

ModelBuilder& ModelBuilder::size(float size)
{
	this->model.setSize(size);
	return this->getInstance();
}

ModelBuilder& ModelBuilder::phongShader()
{
	if (this->model.shader()) {
		//return ModelBuilder::builder;
		return this->getInstance();

	}
	else {
		this->model.shader(new PhongShader());
		//return ModelBuilder::builder;
		return this->getInstance();

	}
}

ModelBuilder& ModelBuilder::ambientColor(const Color& ambient)
{
	if (this->model.shader()) {
		PhongShader* pShader = dynamic_cast<PhongShader*>(this->model.shader());
		if (pShader) {
			pShader->ambientColor(ambient);
		}
		//return ModelBuilder::builder;
		return this->getInstance();


	}
	else {
		return this->phongShader();
	}
}

ModelBuilder& ModelBuilder::diffuseColor(const Color& diffuse)
{
	if (this->model.shader()) {
		PhongShader* pShader = dynamic_cast<PhongShader*>(this->model.shader());
		if (pShader) {
			pShader->diffuseColor(diffuse);
		}
		//return ModelBuilder::builder;
		return this->getInstance();


	}
	else {
		return this->phongShader();
	}
}

Model* ModelBuilder::buildModel(const char* ModelFile, float size)
{
	//bool ret = this->model.load(ModelFile, size);
	//if (!ret)
	//	throw std::exception();
	//this->model.setSize(size);
	//Model* tmp = this->model;
	//this->model = nullptr;
	//return tmp;
	Model* newModel = new Model(ModelFile, this->model);
	this->model = {};
	return newModel;
}

Spieler* ModelBuilder::buildSpieler(const char* ModelFile, int lebensPunkte)
{
	Spieler* newSpieler = new Spieler(ModelFile, lebensPunkte, this->model);
	this->model = {};
	return newSpieler;
}

Gegner* ModelBuilder::buildGegner(const char* ModelFile)
{
	Gegner* newGegner = new Gegner(ModelFile, this->model);
	this->model = {};
	return newGegner;
}

Bullet* ModelBuilder::buildBullet(const char* ModelFile, PointLight* light)
{
	Bullet* newBullet = new Bullet(ModelFile, this->model, light);
	this->model = { };
	return newBullet;
}
