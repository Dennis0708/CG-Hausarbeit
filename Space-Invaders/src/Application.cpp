//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"
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
#include "lineplanemodel.h"
#include "triangleplanemodel.h"
#include "trianglespheremodel.h"
#include "lineboxmodel.h"
#include "triangleboxmodel.h"
#include "model.h"
#include "ShaderLightmapper.h"




#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif


Application::Application(GLFWwindow* pWin) : bulletQueue(new queue<Bullet*>()), collisionList(new list<Model*>()), pWindow(pWin), Cam(pWin), pModel(NULL), ShadowGenerator(2048, 2048)
{
	Cam.setPosition(Vector(0, 0, 10));


	Vector min, max, minO, maxO;
	min = this->calc3DRay(0, 800, minO);
	max = this->calc3DRay(600, 0, maxO);

	minO = maxO = Vector(0, 0, 10);

	Vector ebene = Vector(0, 0, 1);
	float maxS = -ebene.dot(maxO) / ebene.dot(max);
	float minS = -ebene.dot(minO) / ebene.dot(min);

	Vector maxRay = maxO + max * maxS;
	Vector minRay = minO + min * minS;
	cout << maxRay.toString() << " : " << minRay.toString() << endl;

	this->feld = new AABB(minRay, maxRay);

	createGame();
}

void Application::start()
{
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Application::update(float dtime)
{
	Cam.update();
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE)) {
		exit(0);
	}
	int links = glfwGetKey(pWindow, GLFW_KEY_LEFT);
	int rechts = glfwGetKey(pWindow, GLFW_KEY_RIGHT);
	bool shotFired = glfwGetKey(pWindow, GLFW_KEY_SPACE);

	spieler->steuern(rechts - links);
	this->collisionFeld();
	spieler->update(dtime);
	if (shotFired) {
		this->spieler->shoot();
	}

}

void Application::collisionFeld()
{
	for (Model* m : *this->collisionList) {
		Vector minHitbox = m->boundingBox().Min + m->transform().translation();
		Vector maxHitbox = m->boundingBox().Max + m->transform().translation();
		//cout << minHitbox.toString() << " : " << maxHitbox.toString() << endl;
		if (this->feld->Min.X > minHitbox.X) {
			m->collisionFeld(WEST);
		}
		else if (this->feld->Min.Y > minHitbox.Y) {
			m->collisionFeld(SOUTH);
		}
		else if (this->feld->Max.X < maxHitbox.X) {
			m->collisionFeld(EAST);
		}
		else if (this->feld->Max.Y < maxHitbox.X) {
			m->collisionFeld(NORTH);
		}
	}
}

void Application::collisionBullet()
{
	for (auto m : *this->collisionList) {

	}
}

void Application::draw()
{
	ShadowGenerator.generate(Models);

	// 1. clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
	// 2. setup shaders and draw models
	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
	{
		(*it)->draw(Cam);
	}
	ShaderLightMapper::instance().deactivate();

	// 3. check once per frame for opengl errors
	GLenum Error = glGetError();
	assert(Error == 0);
}

void Application::end()
{
	for (ModelList::iterator it = Models.begin(); it != Models.end(); ++it)
		delete* it;

	Models.clear();
}

void Application::createGame()
{
	Matrix m;
	PhongShader* pShader = new PhongShader();

	pModel = new TriangleBoxModel(this->feld->Max.X - this->feld->Min.X, this->feld->Max.Y - this->feld->Min.Y, 0);
	pShader->ambientColor(Color(1, 1, 1));
	pModel->shader(pShader, true);
	m.translation(0, 0, 0);
	pModel->transform(m);
	Models.push_back(pModel);

	Bullet* pBullet = new Bullet(ASSET_DIRECTORY "Sting-Sword-lowpoly.dae", Cam.position() + Vector(0, 0, 10), 1, 10);
	pShader = new PhongShader();
	pBullet->shader(pShader, true);
	this->collisionList->push_back(pBullet);

	spieler = new Spieler(ASSET_DIRECTORY "Space_Invader.obj", Vector(0, -6, 0), 0.006f, 10, pBullet);
	spieler->shader(new PhongShader(), true);
	Models.push_back(spieler);
	this->collisionList->push_back(spieler);


	int maxBullets = 10;
	for (int i = 0; i < maxBullets; i++) {
		pBullet = new Bullet(ASSET_DIRECTORY "Space_Invader.obj", Cam.position() + Vector(0, 0, 10), 1, 2);
		pShader = new PhongShader();
		pShader->ambientColor(Color(0.5f, 0.5f, 0));
		pBullet->shader(pShader, true);
		this->bulletQueue->push(pBullet);
		Models.push_back(pBullet);
		this->collisionList->push_back(pBullet);
	}

	// directional lights
	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(0.2f, -1, 1));
	dl->color(Color(0.25, 0.25, 0.5));
	dl->castShadows(true);
	ShaderLightMapper::instance().addLight(dl);
}

Vector Application::calc3DRay(float x, float y, Vector& Pos)
{
	int width, height;
	glfwGetWindowSize(pWindow, &width, &height);
	x = 2 * (x / width) - 1;
	y = -(2 * (y / height) - 1);

	Vector richtung(x, y, 0);
	Matrix projectionInverted(Cam.getProjectionMatrix());
	projectionInverted.invert();
	richtung = projectionInverted * richtung;
	Matrix viewInverted = Cam.getViewMatrix();
	viewInverted.invert();
	richtung = viewInverted.transformVec3x3(richtung);
	Pos = viewInverted.translation();
	return richtung;
}