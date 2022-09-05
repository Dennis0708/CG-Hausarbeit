#pragma once

#include "Model.h"
#include "Spieler.h"
#include "Gegner.h"
#include "Menu.h"
#include "GameBar.h"
#include "TriangleBoxModel.h"
#include "ConstantShader.h"
#include "Background.h"


class ModelBuilder
{
	//static ModelBuilder builder;
	const std::string ModelFile;
	BaseModel model;
	ModelBuilder() = default;
	//Model* getModel();

public:
	static ModelBuilder& getInstance();
	//ModelBuilder& loadModel(const char* ModelFile, float size = 1);
	ModelBuilder& position(const Vector& pos);
	ModelBuilder& size(float size);
	ModelBuilder& constantShader();
	ModelBuilder& color(const Color& c);
	ModelBuilder& phongShader();
	ModelBuilder& ambientColor(const Color& ambient);
	ModelBuilder& diffuseColor(const Color& diffuse);
	//ModelBuilder& diffuseTexture(const Texture* pTex);
	ModelBuilder& castShadows(bool doesCast);
	Model* buildModel(const char* ModelFile, float size = 1);
	Spieler* buildSpieler(const char* ModelFile, int lebensPunkte);
	Gegner* buildGegner(const char* ModelFile);
	Bullet* buildBullet(const char* ModelFile, PointLight* light = nullptr);
	TriangleBoxModel* buildTriangleBoxModel(float Width = 1, float Height = 1, float Depth = 1);
	Menu* buildMenu(float Width = 1, float Height = 1, float Depth = 1);
	GameBar* buildGameBar(list<Model*>* lebensPunkte, const Vector ursprung, float Width = 1, float Height = 1, float Depth = 1);
	Background* buildBackground(float Width = 1, float Height = 1, float Depth = 1);

};