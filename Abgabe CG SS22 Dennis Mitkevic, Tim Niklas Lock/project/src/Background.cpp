#include "Background.h"

Background::Background(float Width, float Height, float Depth) :TriangleBoxModel(Width,Height,Depth)
{

}

Background::Background(const BaseModel& toCopy, float Width, float Height, float Depth) : TriangleBoxModel(toCopy, Width, Height, Depth)
{
}

void Background::draw(const BaseCamera& Cam)
{
	TriangleBoxModel::draw(Cam);
}
