#include "ModelBuilder.h"



ModelBuilder& ModelBuilder::getInstance()
{
	static ModelBuilder instance;
	return instance;
}


ModelBuilder& ModelBuilder::position(const Vector& pos)
{
	this->model.show(pos);
	return this->getInstance();
}

ModelBuilder& ModelBuilder::size(float size)
{
	this->model.setSize(size);
	return this->getInstance();
}

ModelBuilder& ModelBuilder::constantShader()
{
	if (this->model.shader()) {
		return this->getInstance();

	}
	else {
		this->model.shader(new ConstantShader());
		return this->getInstance();

	}
}

ModelBuilder& ModelBuilder::color(const Color& c)
{
	if (this->model.shader()) {
		ConstantShader* cShader = dynamic_cast<ConstantShader*>(this->model.shader());
		if (cShader) {
			cShader->color(c);
		}
		return this->getInstance();
	}
	else {
		this->constantShader();
		return this->color(c);
	}
}

ModelBuilder& ModelBuilder::phongShader()
{
	if (this->model.shader()) {
		return this->getInstance();

	}
	else {
		this->model.shader(new PhongShader());
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
		return this->getInstance();


	}
	else {
		this->phongShader();
		return this->ambientColor(ambient);
	}
}

ModelBuilder& ModelBuilder::diffuseColor(const Color& diffuse)
{
	if (this->model.shader()) {
		PhongShader* pShader = dynamic_cast<PhongShader*>(this->model.shader());
		if (pShader) {
			pShader->diffuseColor(diffuse);
		}
		return this->getInstance();
	}
	else {
		this->phongShader();
		return this->diffuseColor(diffuse);
	}
}

//ModelBuilder& ModelBuilder::diffuseTexture(const Texture* pTex)
//{
//	if (this->model.shader()) {
//		/*PhongShader* pShader = dynamic_cast<PhongShader*>(this->model.shader());*/
//		delete this->model.shader();
//	}
//	PhongShader* pShader = new PhongShader();
//	pShader->diffuseTexture(pTex);
//	this->model.shader(pShader);
//	return this->getInstance();
//}


ModelBuilder& ModelBuilder::castShadows(bool doesCast)
{
	this->model.shadowCaster(doesCast);
	return this->getInstance();
}

Model* ModelBuilder::buildModel(const char* ModelFile, float size)
{
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

TriangleBoxModel* ModelBuilder::buildTriangleBoxModel(float Width, float Height, float Depth)
{
	TriangleBoxModel* newTriangleBoxModel = new TriangleBoxModel(this->model, Width, Height, Depth);
	this->model = {};
	return newTriangleBoxModel;
}

Menu* ModelBuilder::buildMenu(float Width, float Height, float Depth)
{
	Menu* newMenu = new Menu(this->model, Width, Height, Depth);
	this->model = { };
	return newMenu;
}

GameBar* ModelBuilder::buildGameBar(list<Model*>* lebensPunkte, const Vector ursprung, float Width, float Height, float Depth)
{
	GameBar* newGameBar = new GameBar(this->model, lebensPunkte, ursprung, Width, Height, Depth);
	this->model = {};
	return newGameBar;
}

Background* ModelBuilder::buildBackground(float Width, float Height, float Depth)
{
	Background* newBackground = new Background(this->model, Width, Height, Depth);
	this->model = {};
	return newBackground;
}
