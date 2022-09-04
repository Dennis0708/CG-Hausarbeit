#pragma once

#include "Model.h"
#include "Spieler.h"
#include "Gegner.h"


class ModelBuilder
{
	//static ModelBuilder builder;
	const std::string ModelFile;
	Model model;
	ModelBuilder() = default;
	//Model* getModel();

public:
	static ModelBuilder& getInstance();
	//ModelBuilder& loadModel(const char* ModelFile, float size = 1);
	ModelBuilder& position(const Vector& pos);
	ModelBuilder& size(float size);
	ModelBuilder& phongShader();
	ModelBuilder& ambientColor(const Color& ambient);
	ModelBuilder& diffuseColor(const Color& diffuse);
	Model* buildModel(const char* ModelFile, float size = 1);
	Spieler* buildSpieler(const char* ModelFile, int lebensPunkte);
	Gegner* buildGegner(const char* ModelFile);
	Bullet* buildBullet(const char* ModelFile, PointLight* light = nullptr);

};