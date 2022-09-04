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

Postprocessing::Postprocessing(unsigned int Width, unsigned int Height)
{
	// TODO: kein plan ob GL para richtig
	/*bool created = this->renderetScene.create(Width, Height, GL_R32F, GL_RED, GL_FLOAT, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, false);
	if (!created)
		throw std::exception();*/

	
	/*created = this->FrameBuffer.create(true, Width, Height);
	if (!created)
		throw std::exception();*/

		//render and frame buffer objects
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	glGenTextures(1, &colorBuffer);

	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// attatch the tex to frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

	const GLenum a = GL_COLOR_ATTACHMENT0;

	glDrawBuffers(1, &a);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	InitVB();
}

Postprocessing::~Postprocessing()
{
}

void Postprocessing::activate()
{
	// TODO: Bin mir nicht sicher ob glClearColor und glCullFace hier schlau ist
	//glClearColor(1.0f, 0.0, 0.0f, 1);
	//glCullFace(GL_FRONT);

	/*GLint PrevViewport[4];
	glGetIntegerv(GL_VIEWPORT, PrevViewport);
	glViewport(0, 0, renderetScene.width(), renderetScene.height());*/

	/*FrameBuffer.attachColorTarget(renderetScene);
	FrameBuffer.activate();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Postprocessing::deactivate()
{
	/*FrameBuffer.deactivate();
	FrameBuffer.detachColorTarget();*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glClearColor(0, 0, 0, 1);
	//glCullFace(GL_BACK);
	//glViewport(PrevViewport[0], PrevViewport[1], PrevViewport[2], PrevViewport[3]);
}

void Postprocessing::setParameter()
{
	Shader.setTex0(&renderetScene);
}

void Postprocessing::drawPost(const BaseCamera& Cam)
{
	Shader.activate(Cam);

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderetScene.ID());
	
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);*/

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	//render a quad for the frame
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	Shader.deactivate();
}

void Postprocessing::InitVB()
{
	//// configure VAO/VBO
	//unsigned int VBO;
	//float vertices[] = {
	//	// pos        // tex
	//	-1.0f, -1.0f, 0.0f, 0.0f,
	//	 1.0f,  1.0f, 1.0f, 1.0f,
	//	-1.0f,  1.0f, 0.0f, 1.0f,

	//	-1.0f, -1.0f, 0.0f, 0.0f,
	//	 1.0f, -1.0f, 1.0f, 0.0f,
	//	 1.0f,  1.0f, 1.0f, 1.0f
	//};
	//glGenVertexArrays(1, &this->VAO);
	//glGenBuffers(1, &VBO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindVertexArray(this->VAO);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	//glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glBindVertexArray(0);
	unsigned int VBO;
	float quadVertices[] = {
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//render quad
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
