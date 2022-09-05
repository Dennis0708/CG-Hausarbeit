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

/**
* https://learnopengl.com/In-Practice/2D-Game/Postprocessing
*/

class PostprocessingShader : public BaseShader
{
public:
	PostprocessingShader();
	virtual void activate(const BaseCamera& Cam) const;
	void setTex0(Texture* Tex);
	void setShake(bool shake);
	void setTime(float time);
protected:
	Texture* Tex;
	float offsets[9][2];
	float blur_kernel[9];
	bool shake;
	float time;
	GLint Tex0Loc;
	GLint OffsetsLoc;
	GLint BlurKernelLoc;
	GLint ShakeLoc;
	GLint TimeLoc;
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
	void setShakeTime(float time) { this->shakeTime = time; }
	float getShakeTime() { return this->shakeTime; }
	void setShake(bool shake) { this->shake = shake; }
	void InitVB();
protected:
	float shakeTime;
	bool shake;
	Framebuffer FrameBuffer;
	PostprocessingShader* Shader;
	Texture renderetScene;
	VertexBuffer VB;
};

