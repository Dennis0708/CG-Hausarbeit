//
//  LineBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "LineBoxModel.h"

LineBoxModel::LineBoxModel( float Width, float Height, float Depth )
{
    float startx = Width / 2;
    float starty = Height / 2;
    float startz = Depth / 2;

    // 1. setup vertex buffer
    VB.begin();

    VB.addVertex(startx, -starty, -startz);
    VB.addVertex(startx, -starty, startz);
    VB.addVertex(-startx, -starty, -startz);
    VB.addVertex(-startx, -starty, startz);
    VB.addVertex(-startx, -starty, startz);
    VB.addVertex(startx, -starty, startz);
    VB.addVertex(-startx, -starty, -startz);
    VB.addVertex(startx, -starty, -startz);

    VB.addVertex(-startx, -starty, -startz);
    VB.addVertex(-startx, starty, -startz);
    VB.addVertex(startx, -starty, -startz);
    VB.addVertex(startx, starty, -startz);
    VB.addVertex(-startx, -starty, startz);
    VB.addVertex(-startx, starty, startz);
    VB.addVertex(startx, -starty, startz);
    VB.addVertex(startx, starty, startz);

    VB.addVertex(startx, starty, -startz);
    VB.addVertex(startx, starty, startz);
    VB.addVertex(-startx, starty, -startz);
    VB.addVertex(-startx, starty, startz);
    VB.addVertex(-startx, starty, startz);
    VB.addVertex(startx, starty, startz);
    VB.addVertex(-startx, starty, -startz);
    VB.addVertex(startx, starty, -startz);
    
    VB.end();
}

void LineBoxModel::draw(const BaseCamera& Cam)
{
    BaseModel::draw(Cam);

    VB.activate();

    glDrawArrays(GL_LINES, 0, VB.vertexCount());

    VB.deactivate();
}
