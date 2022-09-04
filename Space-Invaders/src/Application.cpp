//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"

Application::Application(GLFWwindow* pWin)
	: pWindow(pWin), Cam(pWin), pModel(NULL), ShadowGenerator(2048, 2048),
	gegnerListe(new list<Gegner*>()), bulletList(new list<Bullet*>()), menu(NULL), gameState(GameState::BEFORE_START),
	barrieren(new list<Barriere*>()), partikelList(new list<TriangleBoxModel*>()), gameBar(NULL),
	lebensPunkte(new list<Model*>()), close(false)
{
	int width, height;
	glfwGetWindowSize(pWindow, &width, &height);

	this->postprocessing = new Postprocessing(width, height);

	Cam.setPosition(Vector(0, 0, 10));
	Cam.update();

	this->createFeld();
	this->createGame();
	this->createLights();
}

void Application::createGame()
{
	Matrix m;

	PointLight* light = new PointLight();
	light->position(Vector(0, 0, 30));
	light->color({ 0,0,0 });
	light->attenuation({ 0.5f,0.1f,0.1f });
	ShaderLightMapper::instance().addLight(light);

	/*Bullet* spielerBullet = new Bullet(ASSET_DIRECTORY "bullet_zylinder.obj", Cam.position() + Vector(0, 0, 10), 0.3f, light);
	pShader = new PhongShader();
	pShader->ambientColor({ 0,0,0 });
	spielerBullet->shader(pShader, true);*/
	Bullet* spielerBullet = ModelBuilder::getInstance().position(Cam.position() + Vector(0, 0, 10)).size(0.3f).
		phongShader().ambientColor({ 0,0,0 }).buildBullet(ASSET_DIRECTORY "bullet_zylinder.obj", light);
	this->castsShadowList.push_back(spielerBullet);

	/*spieler = new Spieler(ASSET_DIRECTORY "Laser_Cannon.obj", Vector(0, boden->transform().translation().Y + boden->boundingBox().size().Y * 0.5f + 0.3f, 0), 0.6f, LEBENSPUNKTE_SPIELER, spielerBullet);
	pShader = new PhongShader();
	pShader->ambientColor({ 0,0,0 });
	spieler->shader(pShader, true);*/
	this->spieler = ModelBuilder::getInstance().phongShader().ambientColor({ 0,0,0 }).size(0.6f).
		position(Vector(0, boden->transform().translation().Y + boden->boundingBox().
			size().Y * 0.5f + 0.3f, 0)).buildSpieler(ASSET_DIRECTORY "Laser_Cannon.obj", LEBENSPUNKTE_SPIELER);
	this->spieler->setBullet(spielerBullet);
	this->drawables.push_back(spieler);
	this->castsShadowList.push_back(spieler);

	int anzahlGegner = 1; // TODO Hier wieder 50
	list<Gegner*>* tmpGegnerList = new list<Gegner*>();
	for (int i = 0; i < anzahlGegner; i++) {
		char* pfad;
		if (i < 10) {
			pfad = ASSET_DIRECTORY "Space_Invader/Space_Invader_Big.obj";
		}
		else if (i >= 10 && i < 30) {
			pfad = ASSET_DIRECTORY "Space_Invader/Space_Invader_Medium.obj";
		}
		else {
			pfad = ASSET_DIRECTORY "Space_Invader/Space_Invader_Small.obj";
		}

		//Gegner* gegner = new Gegner(pfad, Vector(0, 0, 0), 0.006f);
		//pShader = new PhongShader();
		//pShader->ambientColor({ 0,0,0 });
		//gegner->shader(pShader, true);
		Gegner* gegner = ModelBuilder::getInstance().size(0.006f).phongShader().ambientColor({ 0,0,0 }).buildGegner(pfad);
		tmpGegnerList->push_back(gegner);
		this->gegnerListe->push_back(gegner);
		this->castsShadowList.push_back(gegner);
	}
	this->invasion = new Invasion(tmpGegnerList);
	float gegnerWidth = this->gegnerListe->front()->boundingBox().size().X;
	float gegnerHeight = this->gegnerListe->front()->boundingBox().size().Y;
	this->invasion->start(1 /*TODO hier wieder 10*/, Vector(this->feld->Min.X + gegnerWidth, this->feld->Max.Y - gegnerHeight, 0));
	this->drawables.push_back(this->invasion);

	int maxBullets = 10;
	queue<Bullet*>* bulletQueue = new queue<Bullet*>();
	Bullet* pBullet = nullptr;
	for (int i = 0; i < maxBullets; i++) {
		light = new PointLight();
		light->position(Vector(0, 0, 30));
		light->color({ 0,0,0 });
		light->attenuation({ 0.5f,0.1f,0.1f });
		ShaderLightMapper::instance().addLight(light);
		/*pBullet = new Bullet(ASSET_DIRECTORY "bullet_zylinder.obj", Cam.position() + Vector(0, 0, 10), 0.3f, light);
		pShader = new PhongShader();
		pShader->ambientColor({ 0,0,0 });
		pBullet->shader(pShader, true);*/
		pBullet = ModelBuilder::getInstance().position(Cam.position() + Vector(0, 0, 10)).size(0.3f).
			phongShader().ambientColor({ 0,0,0 }).buildBullet(ASSET_DIRECTORY "bullet_zylinder.obj", light);
		this->bulletList->push_back(pBullet);
		bulletQueue->push(pBullet);
		this->castsShadowList.push_back(pBullet);
		pBullet->shadowCaster(false);
	}
	this->invasion->setBulletQueue(bulletQueue);

	int maxBarrieren = 3;
	int maxPartikel = 1; // TODO hier wieder 50
	list<TriangleBoxModel*>* partikelListe;
	TriangleBoxModel* partikel;
	Barriere* barriere;
	float abstand = this->gameWidth * 0.25f;
	for (int i = 0; i < maxBarrieren; i++) {
		partikelListe = new list<TriangleBoxModel*>();
		for (int j = 0; j < maxPartikel; j++) {
			partikel = new TriangleBoxModel(0.2f, 0.2f, 0.2f);
			pShader = new PhongShader();
			//pShader->ambientColor({ 0,0,0 });
			pShader->ambientColor({ 0,0.2,0 });
			pShader->diffuseColor({ 0,0.4,0 });
			pShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "texture/green_bullet.jpg"));
			partikel->shader(pShader, true);
			partikelListe->push_back(partikel);
			this->partikelList->push_back(partikel);
			this->castsShadowList.push_back(partikel);
		}
		barriere = new Barriere(partikelListe);
		barriere->init(1 /*TODO hier wieder 10*/, Vector((-abstand + i * abstand), -3, 0));
		this->barrieren->push_back(barriere);
		this->drawables.push_back(barriere);
	}

	this->menu = new Menu(this->feld->size().X * 0.05f, this->feld->size().Y * 0.08f, 0);
	this->menu->shadowCaster(false);
	pShader = new PhongShader();
	pShader->diffuseColor(Color(0, 0, 0));
	pShader->ambientColor(Color(0, 0, 0.3f));
	this->menu->shader(pShader, true);
	m.translation(Vector(0, 0, 20));
	this->menu->transform(m);
	this->drawables.push_back(menu);
}

void Application::createFeld() {
	int width, height;
	glfwGetWindowSize(pWindow, &width, &height);

	Vector minD, maxD, pos;
	minD = this->calc3DRay(0, height, pos);
	maxD = this->calc3DRay(width, 0, pos);

	Vector ebene = Vector(0, 0, 1);
	float maxS = -ebene.dot(pos) / ebene.dot(maxD);
	float minS = -ebene.dot(pos) / ebene.dot(minD);

	Vector maxRay = pos + maxD * maxS;
	Vector minRay = pos + minD * minS;

	this->feld = new AABB(minRay, maxRay);

	this->gameWidth = this->feld->size().X;
	this->gameHeight = this->feld->size().Y;
	this->collisionDetector = new CollisionDetector(this->feld);

	Matrix m;


	background = new Background((this->feld->Max.X - this->feld->Min.X) * 1.2f, (this->feld->Max.Y - this->feld->Min.Y) * 1.2f, 0);
	pShader = new PhongShader();
	//pShader->ambientColor({ 0.2f,0.2f,0.2f });
	//pShader->ambientColor(Color(0.1f, 0.1f, 0.1f));
	pShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "texture/invasion2.jpg"));
	background->shader(pShader, true);
	m.translation(0, 0, -1);
	background->transform(m);
	drawables.push_back(background);
	this->castsShadowList.push_back(background);

	this->boden = new Background(this->feld->size().X * 1.5f, 1, 5);
	pShader = new PhongShader();
	//	pShader->ambientColor(Color(0.64f, 0.16f, 0.16f));
		//pShader->diffuseColor(Color(0.64f, 0.16f, 0.16f));
	pShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "texture/damagedAsphalt_C_00.dds"));
	this->boden->shader(pShader, true);
	this->boden->show(Vector(this->feld->Min.X + this->feld->size().X * 0.5f, this->feld->Min.Y, 0));
	this->drawables.push_back(boden);
	this->castsShadowList.push_back(boden);

	this->feld->Min.Y += this->boden->boundingBox().size().Y * 0.5f;

	list<Model*>* lebensPunkte = new list<Model*>();
	for (int i = 0; i < LEBENSPUNKTE_SPIELER; i++) {
		Model* lebensPunkt = new Model(ASSET_DIRECTORY "Laser_Cannon.obj", Vector(0, 0, 0), 0.3f);
		lebensPunkt->shadowCaster(false);
		pShader = new PhongShader();
		lebensPunkt->shader(pShader, true);
		lebensPunkte->push_back(lebensPunkt);
		this->lebensPunkte->push_back(lebensPunkt);
		this->castsShadowList.push_back(lebensPunkt);
	}

	float gameBarHeight = this->lebensPunkte->front()->boundingBox().size().Y * 1.5f;

	this->feld->Max.Y = this->feld->Max.Y - gameBarHeight;

	this->gameBar = new GameBar(lebensPunkte, Vector(0, this->feld->Max.Y + gameBarHeight * 0.5, 0), this->gameWidth, gameBarHeight, 0);
	this->gameBar->shadowCaster(false);
	cShader = new ConstantShader();
	cShader->color(Color(0.5f, 0, 0));
	this->gameBar->shader(cShader, true);
	this->gameBar->init();
	drawables.push_back(this->gameBar);
}

void Application::createLights()
{
	//// directional lights
	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(-0.1f, -0.1f, -1));
	dl->color({ 0.4f,0.4f,0.4f });
	dl->castShadows(false);
	ShaderLightMapper::instance().addLight(dl);

	float innerradius = 10;
	float outerradius = 15;
	//Color c = Color(0.75f, 0.75f, 0.75f);
	Color c = { 1,1,1 };

	this->spielerLight = new SpotLight();
	this->spielerLight->position(this->spieler->transform().translation() + Vector(0, 5, 4));
	this->spielerLight->color(c);
	this->spielerLight->direction((this->spieler->transform().translation() - this->spielerLight->position()).normalize());
	this->spielerLight->innerRadius(innerradius);
	this->spielerLight->outerRadius(outerradius);
	this->spielerLight->castShadows(true);
	this->spielerLight->attenuation({ 0.5f,0,0 });
	ShaderLightMapper::instance().addLight(this->spielerLight);

	innerradius = 7;
	outerradius = 10;

	this->searchLight = new Searchlight(*this->invasion, this->feld->Min);
	this->searchLight->position(this->spieler->transform().translation() + Vector(0, 3, 8));
	this->searchLight->color(c);
	this->searchLight->innerRadius(innerradius);
	this->searchLight->outerRadius(outerradius);
	this->searchLight->castShadows(true);
	this->searchLight->attenuation({ 0.5f,0,0 });
	ShaderLightMapper::instance().addLight(this->searchLight);
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

	static float gesamtDtime = 0;
	static float c = 0;
	float avgDtime;
	string s;

	if (glfwGetKey(pWindow, GLFW_KEY_R)) {
		Cam.setPosition(Vector(0, 0, 10));
	}

	switch (this->gameState) {
	case GameState::BEFORE_START:
		this->updateStartscreen();
		break;
	case GameState::GAME_IS_ACTIVE:
		c++;
		gesamtDtime = (gesamtDtime + dtime);
		avgDtime = gesamtDtime / c;
		s = "Computergrafik - Hochschule Osnabrück  FPS: ";
		s.append(to_string(1 / avgDtime));
		glfwSetWindowTitle(pWindow, s.c_str());

		this->updateGame(dtime);
		break;
	case GameState::PAUSE:
		this->updateMenu(dtime);
		break;
	case GameState::RESET:
		this->reset();
		break;
	case GameState::EXIT:
		this->close = true;
	}
}

void Application::updateGame(float dtime)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE)) {
		this->gameState = GameState::PAUSE;
		this->menu->show();
		this->menu->update(dtime);
		this->castsShadowList.push_back(this->menu);
	}
	else {
		int links = glfwGetKey(pWindow, GLFW_KEY_LEFT);
		int rechts = glfwGetKey(pWindow, GLFW_KEY_RIGHT);
		bool shotFired = glfwGetKey(pWindow, GLFW_KEY_SPACE);

		this->spieler->steuern(rechts - links);
		this->spieler->update(dtime);
		this->spieler->shoot(shotFired);
		this->invasion->update(dtime);
		this->updateLights(dtime);

		this->collisionFeld();
		this->collisionBullet();

		if (this->spieler->getLebenspunkte() <= 0) {
			this->gameState = GameState::RESET;
		}
		if (this->invasion->getGegnerListe()->empty()) {
			this->gameState = GameState::RESET;
		}
	}

}

void Application::updateStartscreen()
{
	if (glfwGetKey(pWindow, GLFW_KEY_ENTER)) {
		this->gameState = GameState::GAME_IS_ACTIVE;
	}
}

void Application::updateMenu(float dtime)
{
	if (glfwGetKey(pWindow, GLFW_KEY_UP)) {
		this->menu->up();
	}
	if (glfwGetKey(pWindow, GLFW_KEY_DOWN)) {
		this->menu->down();
	}
	if (glfwGetKey(pWindow, GLFW_KEY_SPACE)) {
		this->gameState = this->menu->enter();
		this->menu->hide();
		this->castsShadowList.remove(this->menu);
	}
	this->menu->update(dtime);
}

void Application::updateLights(float dtime)
{
	this->spielerLight->direction((this->spieler->transform().translation() - this->spielerLight->position()).normalize());

	this->searchLight->update(dtime);
}

void Application::collisionFeld()
{
	Collision collision = this->collisionDetector->borderCollisionGegner(this->invasion->getGegnerListe());
	if (collision != Collision::NONE) {
		if (collision == Collision::DOWN) {
			this->reset();
		}
		this->invasion->collisionBorder(collision, -this->gameHeight * 2.f);
	}
	collision = this->collisionDetector->borderCollision(this->spieler);
	if (collision != Collision::NONE) this->spieler->collisionBorder(collision);
	collision = this->collisionDetector->borderCollision(this->spieler->getBullet());
	if (collision != Collision::NONE) this->spieler->getBullet()->collisionBorder(collision);
	list<Gegner*>* gegnerListe = this->invasion->getGegnerListe();
	for (Gegner* gegner : *gegnerListe) {
		if (gegner->getBullet()) {
			collision = this->collisionDetector->borderCollision(gegner->getBullet());
			if (collision != Collision::NONE) {
				gegner->getBullet()->collisionBorder(collision);
			}
		}
	}
}

void Application::collisionBullet()
{
	Gegner* gegner = (Gegner*)this->collisionDetector->collision(this->spieler->getBullet(), this->invasion->getGegnerListe());
	if (gegner) {
		this->spieler->getBullet()->collisionBullet();
		this->invasion->removeGegner(gegner);
	}
	list<Bullet*>* bullets = this->invasion->getBulletsInGame();
	for (Bullet* bullet : *bullets) {
		if (this->collisionDetector->collision(this->spieler, bullet)) {
			bullet->collisionBullet();
			this->spieler->collisionBullet();
			this->gameBar->removeLife();
		}
		else if (this->spieler->getBullet()->isMoving()) {
			if (this->collisionDetector->collision(this->spieler->getBullet(), bullet)) {
				bullet->collisionBullet();
				this->spieler->getBullet()->collisionBullet();
			}
		}
	}
	for (Bullet* bullet : *bullets) {
		for (Barriere* barriere : *this->barrieren) {
			list<TriangleBoxModel*> collisionList = this->collisionDetector->collision(bullet, barriere->getPartikel());
			if (!collisionList.empty()) {
				TriangleBoxModel* highestPartikel = collisionList.front();
				for (TriangleBoxModel* partikel : collisionList) {
					if (partikel->transform().translation().Y > highestPartikel->transform().translation().Y) {
						highestPartikel = partikel;
					}
				}
				barriere->collisionBullet(highestPartikel);
				bullet->collisionBullet();
			}
		}
	}
	for (Barriere* barriere : *this->barrieren) {
		list<TriangleBoxModel*> collisionList = this->collisionDetector->collision(this->spieler->getBullet(), barriere->getPartikel());
		if (!collisionList.empty()) {
			TriangleBoxModel* lowestPartikel = collisionList.front();
			for (TriangleBoxModel* partikel : collisionList) {
				if (partikel->transform().translation().Y < lowestPartikel->transform().translation().Y) {
					lowestPartikel = partikel;
				}
			}
			barriere->collisionBullet(lowestPartikel);
			this->spieler->getBullet()->collisionBullet();
		}
	}
}

void Application::draw()
{
	ShadowGenerator.generate(this->castsShadowList);

	// 1. clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
	// 2. setup shaders and draw models
	//this->postprocessing->activate();
	for (list<Drawable*>::iterator it = drawables.begin(); it != drawables.end(); ++it)
	{
		(*it)->draw(Cam);
	}
	//this->postprocessing->deactivate();

	//this->postprocessing->drawPost(Cam);

	ShaderLightMapper::instance().deactivate();

	// 3. check once per frame for opengl errors
	GLenum Error = glGetError();
	assert(Error == 0);
}

void Application::end()
{
	for (list<Drawable*>::iterator it = drawables.begin(); it != drawables.end(); ++it)
		delete* it;

	drawables.clear();
}

void Application::reset()
{
	this->gameState = GameState::BEFORE_START;
	this->spieler->reset();
	this->spielerLight->direction((this->spieler->transform().translation() - this->spielerLight->position()).normalize());

	list<Gegner*>* tmpGegnerList = new list<Gegner*>();
	for (Gegner* gegner : *this->gegnerListe) {
		tmpGegnerList->push_back(gegner);
	}
	this->invasion->reset(tmpGegnerList);
	queue<Bullet*>* bulletQueue = new queue<Bullet*>();
	for (Bullet* b : *this->bulletList) {
		b->reset();
		bulletQueue->push(b);
	}
	this->invasion->setBulletQueue(bulletQueue);

	this->searchLight->reset(this->feld->Min);

	int partikelProBarriere = this->partikelList->size() / this->barrieren->size();
	list<Barriere*>::iterator barIter = this->barrieren->begin();
	list<TriangleBoxModel*>* tmpPartikelList = new list<TriangleBoxModel*>();
	int i = 0;
	for (TriangleBoxModel* partikel : *this->partikelList) {
		tmpPartikelList->push_back(partikel);
		i++;
		if (i == partikelProBarriere) {
			i = 0;
			(*barIter)->reset(tmpPartikelList);
			tmpPartikelList = new list<TriangleBoxModel*>();
			barIter++;
		}
	}
	list<Model*>* tmpLebenspunkteList = new list<Model*>();
	for (Model* lebenspunkt : *this->lebensPunkte) {
		tmpLebenspunkteList->push_back(lebenspunkt);
	}
	this->gameBar->reset(tmpLebenspunkteList);
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