#include "Postprocessing.h"

#ifdef WIN32
#define ASSET_GLSL_DIRECTORY "../../assets/GLSL/"
#else
#define ASSET_GLSL_DIRECTORY "../assets/GLSL/"
#endif


PostprocessingBildInvertierenShader::PostprocessingBildInvertierenShader()
{
	bool loaded = load(ASSET_GLSL_DIRECTORY"vspost.glsl", ASSET_GLSL_DIRECTORY"fspost.glsl");
	if (!loaded)
		throw std::exception();

	Tex0Loc = getParameterID("Tex0");
}

void PostprocessingBildInvertierenShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	Tex->activate();
	setParameter(Tex0Loc, (int)Tex->ID());
}

void PostprocessingBildInvertierenShader::setTex0(Texture* Tex)
{
	this->Tex = Tex;
}

Postprocessing::Postprocessing(unsigned int Width, unsigned int Height): Shader(new PostprocessingBildInvertierenShader())
{
	bool created = this->renderetScene.create(Width, Height, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, false);
	if (!created)
		throw std::exception();

	created = this->FrameBuffer.create(true, Width, Height);
	if (!created)
		throw std::exception();

	InitVB();
}

Postprocessing::~Postprocessing()
{
}

void Postprocessing::activate()
{
	// TODO: Bin mir nicht sicher ob glClearColor und glCullFace hier schlau ist
	glClearColor(0.0f, 0.0f, 0.0f, 1);
	glCullFace(GL_FRONT);

	FrameBuffer.attachColorTarget(renderetScene);
	FrameBuffer.activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Postprocessing::deactivate()
{
	FrameBuffer.deactivate();
	FrameBuffer.detachColorTarget();

	glClearColor(0, 0, 0, 1);
	glCullFace(GL_BACK);
}

void Postprocessing::drawPost(const BaseCamera& Cam)
{
	Shader->setTex0(&renderetScene);

	Shader->activate(Cam);
	
	VB.activate();
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, VB.vertexCount());
	
	VB.deactivate();

	Shader->deactivate();
}

void Postprocessing::InitVB()
{
	VB.begin();

	VB.addTexcoord0(0.0f, 1.0f);
	VB.addVertex(-1.0f, 1.0f, 0.0f);

	VB.addTexcoord0(0.0f, 0.0f);
	VB.addVertex(-1.0f, -1.0f, 0.0f);

	VB.addTexcoord0(1.0f, 1.0f);
	VB.addVertex(1.0f, 1.0f, 0.0f);

	VB.addTexcoord0(1.0f, 0.0f);
	VB.addVertex(1.0f, -1.0f, 0.0f);

	VB.end();
}
