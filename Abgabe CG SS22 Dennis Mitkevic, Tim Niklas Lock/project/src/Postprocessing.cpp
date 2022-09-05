#include "Postprocessing.h"

#ifdef WIN32
#define ASSET_GLSL_DIRECTORY "../../assets/GLSL/"
#else
#define ASSET_GLSL_DIRECTORY "../assets/GLSL/"
#endif


PostprocessingShader::PostprocessingShader()
{
	bool loaded = load(ASSET_GLSL_DIRECTORY"vspost.glsl", ASSET_GLSL_DIRECTORY"fspost.glsl");
	if (!loaded)
		throw std::exception();

	Tex0Loc = getParameterID("Tex0");
	OffsetsLoc = getParameterID("offsets");
	BlurKernelLoc = getParameterID("blur_kernel");
	ShakeLoc = getParameterID("shake");
	TimeLoc = getParameterID("time");
}

void PostprocessingShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	Tex->activate();
	setParameter(Tex0Loc, (int)Tex->ID());

	float offset = 1.0f / 300.0f;
	float offsets[9][2] = {
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center - right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right    
	};
	glUniform2fv(OffsetsLoc, 9, (float*)offsets);

	float blur_kernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};

	glUniform1fv(BlurKernelLoc, 9, blur_kernel);

	setParameter(ShakeLoc, shake);
	setParameter(TimeLoc, time);
}

void PostprocessingShader::setTex0(Texture* Tex)
{
	this->Tex = Tex;
}

void PostprocessingShader::setShake(bool shake)
{
	this->shake = shake;
}

void PostprocessingShader::setTime(float time)
{
	this->time = time;
}

Postprocessing::Postprocessing(unsigned int Width, unsigned int Height): Shader(new PostprocessingShader())
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
	Shader->setShake(shake);
	Shader->setTime(shakeTime);

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
