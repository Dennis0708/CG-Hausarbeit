#pragma once
#include "Camera.h"

class Drawable
{
public:
	virtual ~Drawable() = default;
	virtual void draw(const BaseCamera& Cam) = 0;
};