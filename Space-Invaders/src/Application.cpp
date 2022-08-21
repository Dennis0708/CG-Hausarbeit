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


Application::Application(GLFWwindow* pWin) : collisionList(new list<Model*>()), pWindow(pWin), Cam(pWin), pModel(NULL), ShadowGenerator(2048, 2048)
{
	Cam.setPosition(Vector(0, 0, 10));

	int width, height;
	glfwGetWindowSize(pWindow, &width, &height);


	Vector minD, maxD, pos;
	minD = this->calc3DRay(0, height, pos);
	maxD = this->calc3DRay(width, 0, pos);

	pos = Vector(0, 0, 10);

	Vector ebene = Vector(0, 0, 1);
	float maxS = -ebene.dot(pos) / ebene.dot(maxD);
	float minS = -ebene.dot(pos) / ebene.dot(minD);

	Vector maxRay = pos + maxD * maxS;
	Vector minRay = pos + minD * minS;

	this->feld = new AABB(minRay, maxRay);
	this->gameWidth = this->feld->size().X;
	this->gameHeight = this->feld->size().Y;
	this->collisionDetector = new CollisionDetector(this->feld);

	this->createGame();
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

	this->spieler->steuern(rechts - links);
	this->spieler->update(dtime);
	if (shotFired) {
		this->spieler->shoot();
	}
	this->invasion->update(dtime);

	this->collisionFeld();
	this->collisionBullet();

}

void Application::collisionFeld()
{
	Collision collision = this->collisionDetector->borderCollisionGegner(this->invasion->getGegnerListe());
	if (collision != Collision::NONE) {
		this->invasion->collisionBorder(collision, -this->gameHeight * 2.f);
	}
	collision = this->collisionDetector->borderCollision(this->spieler);
	if (collision != Collision::NONE) this->spieler->collisionBorder(collision);
	collision = this->collisionDetector->borderCollision(this->spieler->getBullet());
	if (collision != Collision::NONE) this->spieler->getBullet()->collisionBorder(collision);
}

void Application::collisionBullet()
{
	/*queue<Bullet*>* gegnerBullets = this->invasion->getBulletQueue();
	Bullet* spielerBullet = this->spieler->getBullet();
	Vector minBulletHitbox = spielerBullet->boundingBox().Min + spielerBullet->transform().translation();
	Vector maxBulletHitbox = spielerBullet->boundingBox().Max + spielerBullet->transform().translation();
	if (spielerBullet->transform().translation().Z == 0) {
		bool treffer = false;
		for (Model* m : *this->collisionList) {
			if (spielerBullet != m && !treffer && m->transform().translation().Z == 0) {
				Vector minHitbox = m->boundingBox().Min + m->transform().translation();
				Vector maxHitbox = m->boundingBox().Max + m->transform().translation();
				if (maxBulletHitbox.Y > minHitbox.Y && minBulletHitbox.Y < maxHitbox.Y)
					if (maxBulletHitbox.X > minHitbox.X && minBulletHitbox.X < maxHitbox.X) {
						m->collisionBullet(1);
						spielerBullet->collisionBullet(1);
						treffer = true;
					}
			}

		}
	}*/
	this->collisionDetector->collision(this->spieler->getBullet(), this->invasion->getGegnerListe());
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

	Bullet* pBullet = new Bullet(ASSET_DIRECTORY "US_Dollar_Sign/US_Dollar_Sign.obj", Cam.position() + Vector(0, 0, 10), 0.01f, 10);
	pShader = new PhongShader();
	pBullet->shader(pShader, true);
	this->collisionList->push_back(pBullet);

	spieler = new Spieler(ASSET_DIRECTORY "Space_Invader.obj", Vector(0, -6, 0), 0.006f, 10, pBullet);
	spieler->shader(new PhongShader(), true);
	Models.push_back(spieler);
	this->collisionList->push_back(spieler);

	list<Gegner*>* gegnerListe = new list<Gegner*>();
	int anzahlGegner = 20;
	for (int i = 0; i < anzahlGegner; i++) {
		Gegner* gegner = new Gegner(ASSET_DIRECTORY "Space_Invader.obj", Vector(0, 0, 0), 0.006f, 1);
		gegner->shader(new PhongShader(), true);
		Models.push_back(gegner);
		this->collisionList->push_back(gegner);
		gegnerListe->push_back(gegner);
	}
	this->invasion = new Invasion(gegnerListe);
	this->invasion->start(this->gameHeight, 5, Vector(-4, 2, 0));

	/*pModel = new LineBoxModel(spieler->boundingBox().size().X, spieler->boundingBox().size().Y, spieler->boundingBox().size().Z);
	pShader = new PhongShader();
	pShader->ambientColor(Color(1, 0, 0));
	pModel->shader(pShader, true);
	Models.push_back(pModel);*/
	int maxBullets = 10;
	queue<Bullet*>* bulletQueue = new queue<Bullet*>();
	for (int i = 0; i < maxBullets; i++) {
		pBullet = new Bullet(ASSET_DIRECTORY "US_Dollar_Sign/US_Dollar_Sign.obj", Cam.position() + Vector(0, 0, 10), 0.01f, 2);
		pShader = new PhongShader();
		pShader->ambientColor(Color(0.5f, 0.5f, 0));
		pBullet->shader(pShader, true);
		bulletQueue->push(pBullet);
		Models.push_back(pBullet);
		this->collisionList->push_back(pBullet);
	}
	this->invasion->setBulletQueue(bulletQueue);
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