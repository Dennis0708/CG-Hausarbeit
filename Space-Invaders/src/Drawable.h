#pragma once
#include "Camera.h"

class Drawable
{
public:
	virtual void draw(const BaseCamera& Cam) = 0;
};