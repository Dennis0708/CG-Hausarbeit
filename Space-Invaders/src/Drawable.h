#pragma once
class Drawable
{
public:
	virtual void draw(const BaseCamera& Cam) = 0;
};