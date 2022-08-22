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

class Application
{
public:
    typedef std::list<BaseModel*> ModelList;
    Application(GLFWwindow* pWin);
    void start();
    void update(float dtime);
    void collisionFeld();
    void collisionBullet();
    void draw();
    void end();
    Vector calc3DRay(float x, float y, Vector& Pos);
protected:
	void createGame();
    void createFeld();
    Camera Cam;
    ModelList Models;
    GLFWwindow* pWindow;
	BaseModel* pModel;
	ShadowMapGenerator ShadowGenerator;
    float gameWidth;
    float gameHeight;
    Spieler* spieler;
    AABB* feld;
    Invasion* invasion;
    CollisionDetector* collisionDetector;
};

#endif Application_hpp
