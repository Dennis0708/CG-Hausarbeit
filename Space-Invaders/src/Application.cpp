//
//  Application.cpp
//  ogl4
//
//  Created by Philipp Lensing on 16.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Application.h"

Application::Application(GLFWwindow* pWin)
	: pWindow(pWin), Cam(pWin), pModel(NULL), ShadowGenerator(2048, 2048), gegnerListe(new list<Gegner*>()),
	bulletList(new list<Bullet*>()), menu(NULL), gameState(GameState::BEFORE_START),
	barrieren(new list<Barriere*>()), partikelList(new list<TriangleBoxModel*>()), gameBar(NULL),
	lebensPunkte(new list<Model*>()), close(false)
{
	Cam.setPosition(Vector(0, 0, 10));
	Cam.update();

	this->createFeld();
	this->createGame();
}

void Application::createGame()
{
	Matrix m;

	Bullet* pBullet = new Bullet(ASSET_DIRECTORY "bullet_zylinder.obj", Cam.position() + Vector(0, 0, 10), 0.3f);
	pShader = new PhongShader();
	pBullet->shader(pShader, true);

	spieler = new Spieler(ASSET_DIRECTORY "Space_Invader/Space_Invader_Small.obj", Vector(0, -6, 0), 0.006f, LEBENSPUNKTE_SPIELER, pBullet);
	pShader = new PhongShader();
	spieler->shader(pShader, true);
	Models.push_back(spieler);

	int anzahlGegner = 50;
	list<Gegner*>* tmpGegnerList = new list<Gegner*>();
	for (int i = 0; i < anzahlGegner; i++) {
		Gegner* gegner = new Gegner(ASSET_DIRECTORY "Space_Invader/Space_Invader_Small.obj", Vector(0, 0, 0), 0.006f);
		pShader = new PhongShader();
		gegner->shader(pShader, true);
		Models.push_back(gegner);
		tmpGegnerList->push_back(gegner);
		this->gegnerListe->push_back(gegner);
	}
	this->invasion = new Invasion(tmpGegnerList);
	float gegnerWidth = this->gegnerListe->front()->boundingBox().size().X;
	float gegnerHeight = this->gegnerListe->front()->boundingBox().size().Y;
	this->invasion->start(10, Vector(this->feld->Min.X + gegnerWidth, this->feld->Max.Y - this->gameBar->boundingBox().size().Y - gegnerHeight, 0));

	int maxBullets = 10;
	queue<Bullet*>* bulletQueue = new queue<Bullet*>();
	for (int i = 0; i < maxBullets; i++) {
		pBullet = new Bullet(ASSET_DIRECTORY "bullet_prisma.obj", Cam.position() + Vector(0, 0, 10), 0.6f);
		pShader = new PhongShader();
		pBullet->shader(pShader, true);
		this->bulletList->push_back(pBullet);
		bulletQueue->push(pBullet);
		Models.push_back(pBullet);
	}
	this->invasion->setBulletQueue(bulletQueue);

	int maxBarrieren = 3;
	int maxPartikel = 50;
	list<TriangleBoxModel*>* partikelListe;
	TriangleBoxModel* partikel;
	Barriere* barriere;
	float abstand = this->gameWidth * 0.25f;
	for (int i = 0; i < maxBarrieren; i++) {
		partikelListe = new list<TriangleBoxModel*>();
		for (int j = 0; j < maxPartikel; j++) {
			partikel = new TriangleBoxModel(0.2f, 0.2f, 0.2f);
			pShader = new PhongShader();
			pShader->ambientColor(Color(0,0,0));
			pShader->diffuseColor(Color(0, 0, 0));
			partikel->shader(pShader, true);
			partikelListe->push_back(partikel);
			this->partikelList->push_back(partikel);
			Models.push_back(partikel);
		}
		barriere = new Barriere(partikelListe);
		barriere->init(10, Vector((-abstand + i * abstand), -3, 0));
		this->barrieren->push_back(barriere);
	}

	this->menu = new Menu(this->feld->size().X * 0.3f, this->feld->size().Y * 0.5f, 0);
	ConstantShader* cShader = new ConstantShader();
	cShader->color(Color(0, 0, 0.5f));
	this->menu->shader(cShader, true);
	m.translation(Vector(0, 0, 20));
	this->menu->transform(m);
	Models.push_back(this->menu);

	// directional lights
	DirectionalLight* dl = new DirectionalLight();
	dl->direction(Vector(0.2f, -1, -1));
	dl->color(Color(0.25f, 0.25f, 0.5f));
	dl->castShadows(false);
	ShaderLightMapper::instance().addLight(dl);



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

	pModel = new TriangleBoxModel((this->feld->Max.X - this->feld->Min.X) * 1.2f, (this->feld->Max.Y - this->feld->Min.Y) * 1.2f, 0);
	pShader = new PhongShader();
	//pShader->ambientColor(Color(0, 0.5f, 0.1f));
	pShader->diffuseTexture(Texture::LoadShared(ASSET_DIRECTORY "texture/dirtyWalkwayBorder_C_00.dds"));
	pModel->shader(pShader, true);
	m.translation(0, 0, -1);
	pModel->transform(m);
	Models.push_back(pModel);

	list<Model*>* lebensPunkte = new list<Model*>();
	for (int i = 0; i < LEBENSPUNKTE_SPIELER; i++) {
		Model* lebensPunkt = new Model(ASSET_DIRECTORY "Space_Invader/Space_Invader_Small.obj", Vector(0, 0, 0), 0.006f);
		pShader = new PhongShader();
		lebensPunkt->shader(pShader, true);
		Models.push_back(lebensPunkt);
		lebensPunkte->push_back(lebensPunkt);
		this->lebensPunkte->push_back(lebensPunkt);
	}

	float gameBarHeight = this->lebensPunkte->front()->boundingBox().size().Y * 2;
	this->gameBar = new GameBar(lebensPunkte, Vector(0, this->feld->Max.Y - gameBarHeight * 0.5f, 0), this->gameWidth, gameBarHeight, 0);
	cShader = new ConstantShader();
	cShader->color(Color(1, 0, 0));
	this->gameBar->shader(cShader, true);
	this->gameBar->init();
	Models.push_back(this->gameBar);
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
	//cout << 1/dtime << endl;
	Cam.update();

	if (glfwGetKey(pWindow, GLFW_KEY_R)) {
		Cam.setPosition(Vector(0, 0, 10));
	}

	switch (this->gameState) {
	case GameState::BEFORE_START:
		this->updateStartscreen();
		break;
	case GameState::GAME_IS_ACTIVE:
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
		this->menu->update();
		this->lastMenuInput = 0;
	}
	else {
		int links = glfwGetKey(pWindow, GLFW_KEY_LEFT);
		int rechts = glfwGetKey(pWindow, GLFW_KEY_RIGHT);
		bool shotFired = glfwGetKey(pWindow, GLFW_KEY_SPACE);

		this->spieler->steuern(rechts - links);
		this->spieler->update(dtime);
		this->spieler->shoot(shotFired);

		this->invasion->update(dtime);

		this->collisionFeld();
		this->collisionBullet();

		if (this->spieler->getLebenspunkte() <= 0) {
			this->reset();
		}
		if (this->invasion->getGegnerListe()->empty()) {
			this->reset();
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
	this->lastMenuInput += dtime;
	if (this->lastMenuInput >= 0.1f) {
		if (glfwGetKey(pWindow, GLFW_KEY_UP)) {
			this->menu->up();
			this->menu->update();
		}
		if (glfwGetKey(pWindow, GLFW_KEY_DOWN)) {
			this->menu->down();
			this->menu->update();
		}
		this->lastMenuInput = 0;
	}
	if (glfwGetKey(pWindow, GLFW_KEY_SPACE)) {
		this->gameState = this->menu->enter();
		this->menu->hide();
	}
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
				this->invasion->addBullet(gegner->getBullet());
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
			this->invasion->addBullet(bullet);
			this->gameBar->removeLife();
		}
		else if (this->spieler->getBullet()->isMoving()) {
			if (this->collisionDetector->collision(this->spieler->getBullet(), bullet)) {
				bullet->collisionBullet();
				this->spieler->getBullet()->collisionBullet();
				this->invasion->addBullet(bullet);
			}
		}
	}
	for (Bullet* bullet : *bullets) {
		for (Barriere* barriere : *this->barrieren) {
			TriangleBoxModel* partikel = (TriangleBoxModel*)this->collisionDetector->collision(bullet, barriere->getPartikel());
			if (partikel) {
				barriere->collisionBullet(partikel);
				bullet->collisionBullet();
				this->invasion->addBullet(bullet);
			}
		}
	}
	for (Barriere* barriere : *this->barrieren) {
		TriangleBoxModel* partikel = (TriangleBoxModel*)this->collisionDetector->collision(this->spieler->getBullet(), barriere->getPartikel());
		if (partikel) {
			barriere->collisionBullet(partikel);
			this->spieler->getBullet()->collisionBullet();
		}
	}
}

void Application::draw()
{
	ShadowGenerator.generate(Models);

	// 1. clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderLightMapper::instance().activate();
	// 2. setup shaders and draw models
	for (list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it)
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
	for (list<BaseModel*>::iterator it = Models.begin(); it != Models.end(); ++it)
		delete* it;

	Models.clear();
}

void Application::reset()
{
	this->gameState = GameState::BEFORE_START;
	this->spieler->reset();
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