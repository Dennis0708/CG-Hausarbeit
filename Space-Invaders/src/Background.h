#pragma once
#include "TriangleBoxModel.h"
#include "Drawable.h"

class Background: public TriangleBoxModel, public Drawable
{
public:
    Background(float Width = 1, float Height = 1, float Depth = 1);
    virtual ~Background() = default;
    void draw(const BaseCamera& Cam) override;
};

