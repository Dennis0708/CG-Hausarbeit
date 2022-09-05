#include "Menu.h"

#ifdef WIN32
#define ASSET_TEXTURE_DIRECTORY "../../assets/texture/"
#else
#define ASSET_TEXTURE_DIRECTORY "../assets/texture/"
#endif

Menu::Menu(float Width, float Height, float Depth)
	: TriangleBoxModel(Width, Height, Depth), state(MenuState::RESUME), height(Height), lastInput(0)
{
	this->init(Width, Height, Depth);
}

Menu::Menu(const BaseModel& toCopy, float Width, float Height, float Depth) : TriangleBoxModel(toCopy, Width, Height, Depth), state(MenuState::RESUME), height(Height), lastInput(0)

{
	this->init(Width, Height, Depth);
}

Menu::~Menu()
{
	delete this->resume;
	delete this->reset;
	delete this->exit;
	delete this->current;
}

void Menu::init(float Width, float Height, float Depth)
{
	PhongShader* pShader;

	float boxHeight = 0.2f, boxWidth = 0.8f;

	this->resume = new TriangleBoxModel(Width * boxWidth, Height * boxHeight, 0);
	pShader = new PhongShader();
	pShader->diffuseColor(Color(0, 0, 0));
	pShader->diffuseTexture(Texture::LoadShared(ASSET_TEXTURE_DIRECTORY "RESUME.png"));
	this->resume->shader(pShader, true);

	this->reset = new TriangleBoxModel(Width * boxWidth, Height * boxHeight, 0);
	pShader = new PhongShader();
	pShader->diffuseColor(Color(0, 0, 0));
	pShader->diffuseTexture(Texture::LoadShared(ASSET_TEXTURE_DIRECTORY "RESET.png"));
	this->reset->shader(pShader, true);

	this->exit = new TriangleBoxModel(Width * boxWidth, Height * boxHeight, 0);
	pShader = new PhongShader();
	pShader->diffuseColor(Color(0, 0, 0));
	pShader->diffuseTexture(Texture::LoadShared(ASSET_TEXTURE_DIRECTORY "EXIT.png"));
	this->exit->shader(pShader, true);

	this->current = new TriangleBoxModel(Width * (boxWidth + 0.05f), Height * (boxHeight + 0.05f), 0);
	pShader = new PhongShader();
	pShader->diffuseColor(Color(0, 0, 0));
	pShader->ambientColor(Color(0.5f, 0, 0));
	this->current->shader(pShader, true);
}

void Menu::up()
{
	if (this->lastInput > 0.3f) {
		switch (this->state) {
		case MenuState::RESUME:
			this->state = MenuState::EXIT;
			break;
		case MenuState::RESET:
			this->state = MenuState::RESUME;
			break;
		case MenuState::EXIT:
			this->state = MenuState::RESET;
			break;
		}
		this->lastInput = 0;
	}
}

void Menu::down()
{
	if (this->lastInput > 0.1f) {
		switch (this->state) {
		case MenuState::RESUME:
			this->state = MenuState::RESET;
			break;
		case MenuState::RESET:
			this->state = MenuState::EXIT;
			break;
		case MenuState::EXIT:
			this->state = MenuState::RESUME;
			break;
		}
		this->lastInput = 0;
	}
}

GameState Menu::enter()
{
	switch (this->state) {
	case MenuState::RESUME:
		return GameState::GAME_IS_ACTIVE;
	case MenuState::RESET:
		return GameState::RESET;
	case MenuState::EXIT:
		return GameState::EXIT;
	}
}

void Menu::update(float dtime)
{
	this->lastInput += dtime;
	Matrix posMat;
	switch (this->state) {
	case MenuState::RESUME:
		posMat.translation(this->resume->transform().translation() + Vector(0, 0, -0.05f));
		this->current->transform(posMat);
		break;
	case MenuState::RESET:
		posMat.translation(this->reset->transform().translation() + Vector(0, 0, -0.05f));
		this->current->transform(posMat);
		break;
	case MenuState::EXIT:
		posMat.translation(this->exit->transform().translation() + Vector(0, 0, -0.05f));
		this->current->transform(posMat);
		break;
	}
}

void Menu::show()
{
	this->state = MenuState::RESUME;

	Matrix posMat;
	posMat.translation(0, 0, 9);

	this->transform(posMat);
}

void Menu::hide()
{
	Matrix posMat;
	posMat.translation(0, 0, 20);

	this->transform(posMat);
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
