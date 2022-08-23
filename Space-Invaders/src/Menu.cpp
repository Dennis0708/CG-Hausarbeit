#include "Menu.h"

Menu::Menu(float Width, float Height, float Depth) 
	: TriangleBoxModel(Width, Height, Depth), state(MenuState::RESUME), height(Height)
{
	PhongShader* pShader;
	
	float boxHeight = 0.1f, boxWidth = 0.6f;

	this->resume = new TriangleBoxModel(Width * boxWidth, Height * boxHeight, 0);
	pShader = new PhongShader();
	pShader->ambientColor(Color(0, 0.5f, 0));
	//pShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "texture/dirtyWalkwayBorder_C_00.dds"));
	this->resume->shader(pShader, true);
	
	this->reset = new TriangleBoxModel(Width * boxWidth, Height * boxHeight, 0);
	pShader = new PhongShader();
	pShader->ambientColor(Color(0, 0.5f, 0));
	//pShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "texture/dirtyWalkwayBorder_C_00.dds"));
	this->reset->shader(pShader, true);
	
	this->exit = new TriangleBoxModel(Width * boxWidth, Height * boxHeight, 0);
	pShader = new PhongShader();
	pShader->ambientColor(Color(0, 0.5f, 0));
	//pShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "texture/dirtyWalkwayBorder_C_00.dds"));
	this->exit->shader(pShader, true);

	this->current = new TriangleBoxModel(Width * (boxWidth + 0.05f) , Height * (boxHeight + 0.05f), 0);
	pShader = new PhongShader();
	pShader->ambientColor(Color(0.5f, 0, 0));
	//pShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "texture/dirtyWalkwayBorder_C_00.dds"));
	this->current->shader(pShader, true);
}

void Menu::up()
{
}

void Menu::down()
{
}

GameState Menu::enter()
{
	return GameState();
}

void Menu::transform(const Matrix& m)
{
	BaseModel::transform(m);

	Matrix mat;
	mat.translation(Vector(0, (this->height / 4), 0.1f));
	this->resume->transform(m * mat);

	mat.translation(Vector(0, 0, 0.1f));
	this->reset->transform(m * mat);

	mat.translation(Vector(0, -(this->height / 4), 0.1f));
	this->exit->transform(m * mat);

	mat.translation(Vector(0, 0, -0.05f));
	this->current->transform(this->resume->transform() * mat);
}

void Menu::draw(const BaseCamera& Cam)
{
	TriangleBoxModel::draw(Cam);
	this->resume->draw(Cam);
	this->reset->draw(Cam);
	this->exit->draw(Cam);
	this->current->draw(Cam);
}
