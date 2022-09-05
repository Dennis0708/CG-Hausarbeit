//
//  Application.hpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef Application_hpp
#define Application_hpp

#include <stdio.h>
#include <list>
#include "camera.h"
#include "phongshader.h"
#include "constantshader.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "basemodel.h"
#include "ShadowMapGenerator.h"
#include "Spieler.h"
#include "Gegner.h"
#include "Bullet.h"
#include "Aabb.h"
#include <queue>
#include "Invasion.h"
#include "CollisionDetector.h"
#include "Menu.h"
#include "GameState.h"
#include "Barriere.h"
#include "GameBar.h"
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "model.h"
#include "ShaderLightmapper.h"
#include "Drawable.h"
#include "Background.h"
#include "Searchlight.h"
#include "Postprocessing.h"
#include "ModelBuilder.h"

#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#define _USE_MATH_DEFINES
#include <math.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif

#define LEBENSPUNKTE_SPIELER 3

class Application
{
    list<BaseModel*> castsShadowList;
    list<Drawable*> drawables;
    list<Gegner*>* gegnerListe;
    list<Bullet*>* bulletList;
    list<Barriere*>* barrieren;
    list<TriangleBoxModel*>* partikelList;
    list<Model*>* lebensPunkte;

    GLFWwindow* pWindow;
    BaseModel* pModel;
    PhongShader* pShader;
    ConstantShader* cShader;
    Camera Cam;
    ShadowMapGenerator ShadowGenerator;
    Postprocessing* postprocessing;
    Spieler* spieler;
    AABB* feld;
    Invasion* invasion;
    CollisionDetector* collisionDetector;
    Menu* menu;
    GameBar* gameBar;
    Background* background;
    Background* boden;
    GameState gameState;
    SpotLight* spielerLight;
    Searchlight* searchLight;
    float gameWidth;
    float gameHeight;
    bool close;
public:
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    void draw();
    bool isClose() { return this->close; };
    void end();
private:
	void createGame();
    void createFeld();
    void createLights();
    void updateGame(float dtime);
    void updateStartscreen();
    void updateMenu(float dtime);
    void updateLights(float dtime);
    void collisionFeld();
    void collisionBullet();
    void reset();
    Vector calc3DRay(float x, float y, Vector& Pos);
};

#endif Application_hpp
