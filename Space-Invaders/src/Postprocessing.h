#pragma once

#include "BaseShader.h"
#include "Aabb.h"
#include "Framebuffer.h"
#include "Lights.h"
#include "BaseModel.h"
#include "Texture.h"
#include "Drawable.h"
#include "VertexBuffer.h"
#include <list>

//class PostprocessingShader : public BaseShader
//{
//public:
//	PostprocessingShader();
//	virtual void activate(const BaseCamera& Cam) const;
//protected:
//	// TODO: uniforms setzen
//	GLint EyePosLoc;
//	GLint ModelMatLoc;
//	GLint ModelViewProjMatLoc;
//};

class PostprocessingBildInvertierenShader : public BaseShader
{
public:
	PostprocessingBildInvertierenShader();
	virtual void activate(const BaseCamera& Cam) const;
	void setTex0(Texture* Tex);
protected:
	Texture* Tex;
	GLint Tex0Loc;
};

class Postprocessing
{
public:
	Postprocessing() = default;
	Postprocessing(unsigned int Width, unsigned int Height);
	~Postprocessing();

	void activate();
	void deactivate();
	void drawPost(const BaseCamera& Cam);
	void InitVB();
protected:
	Framebuffer FrameBuffer;
	PostprocessingBildInvertierenShader *Shader;
	Texture renderetScene;
	/*GLuint VAO;
	GLuint FBO;
	GLuint RBO;*/
	VertexBuffer VB;

};

