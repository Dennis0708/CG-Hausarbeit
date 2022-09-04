#include "Postprocessing.h"

#ifdef WIN32
#define ASSET_GLSL_DIRECTORY "../../assets/GLSL/"
#else
#define ASSET_GLSL_DIRECTORY "../assets/GLSL/"
#endif

//PostprocessingShader::PostprocessingShader()
//{
//	bool loaded = load(ASSET_GLSL_DIRECTORY"vspostprocessing.glsl", ASSET_GLSL_DIRECTORY"fspostprocessing.glsl");
//	if (!loaded)
//		throw std::exception();
//	
//}
//
//void PostprocessingShader::activate(const BaseCamera& Cam) const
//{
//}

PostprocessingBildInvertierenShader::PostprocessingBildInvertierenShader()
{
	bool loaded = load(ASSET_GLSL_DIRECTORY"vsbildinvertieren.glsl", ASSET_GLSL_DIRECTORY"fsbildinvertieren.glsl");
	if (!loaded)
		throw std::exception();

	Tex0Loc = getParameterID("Tex0");
}

void PostprocessingBildInvertierenShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	setParameter(Tex0Loc, (int)Tex->ID());
}

void PostprocessingBildInvertierenShader::setTex0(Texture* Tex)
{
	this->Tex = Tex;
}

Postprocessing::Postprocessing(unsigned int Width, unsigned int Height): Shader(new PostprocessingBildInvertierenShader())
{
	// TODO: kein plan ob GL para richtig
	bool created = this->renderetScene.create(Width, Height, GL_R32F, GL_RED, GL_FLOAT, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, false);
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
	glClearColor(1.0f, 0.0, 0.0f, 1);
	glCullFace(GL_FRONT);

	FrameBuffer.attachColorTarget(renderetScene);
	FrameBuffer.activate();
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

	renderetScene.activate();
	
	VB.activate();
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, VB.vertexCount());
	
	VB.deactivate();

	renderetScene.deactivate();

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
