#include "Background.h"

Background::Background(float Width, float Height, float Depth) :TriangleBoxModel(Width,Height,Depth)
{

}

void Background::draw(const BaseCamera& Cam)
{
	TriangleBoxModel::draw(Cam);
}
