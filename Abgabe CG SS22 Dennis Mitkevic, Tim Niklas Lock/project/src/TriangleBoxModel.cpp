//
//  TriangleBoxModel.cpp
//  CGXcode
//
//  Created by Philipp Lensing on 10.10.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "TriangleBoxModel.h"

TriangleBoxModel::TriangleBoxModel(float Width, float Height, float Depth): BoundingBox(Vector(0,0,0),Vector(Width,Height,Depth))
{
	this->init(Width, Height, Depth);
}

TriangleBoxModel::TriangleBoxModel(const BaseModel& toCopy, float Width, float Height, float Depth) : BaseModel(toCopy), BoundingBox(Vector(0, 0, 0), Vector(Width, Height, Depth))
{
	this->init(Width, Height, Depth);
}

void TriangleBoxModel::init(float Width, float Height, float Depth)
{

	float startx = Width * 0.5f;
	float starty = Height * 0.5f;
	float startz = Depth * 0.5f;

	// 1. setup vertex buffer
	VB.begin();

	//oben
	VB.addNormal(0, 1, 0);
	VB.addTexcoord0(0, 0);
	VB.addVertex(startx, starty, -startz); // Index 0
	VB.addTexcoord0(0, 1);
	VB.addVertex(startx, starty, startz); // Index 1
	VB.addTexcoord0(1, 0);
	VB.addVertex(-startx, starty, -startz); // Index 2
	VB.addTexcoord0(1, 1);
	VB.addVertex(-startx, starty, startz); // Index 3
	//unten
	VB.addNormal(0, -1, 0);
	VB.addTexcoord0(0, 0);
	VB.addVertex(-startx, -starty, startz); // Index 4
	VB.addTexcoord0(0, 1);
	VB.addVertex(-startx, -starty, -startz); // Index 5
	VB.addTexcoord0(1, 0);
	VB.addVertex(startx, -starty, startz); // Index 6
	VB.addTexcoord0(1, 1);
	VB.addVertex(startx, -starty, -startz); // Index 7
	//vorne
	VB.addNormal(0, 0, 1);
	VB.addTexcoord0(0, 0);
	VB.addVertex(startx, starty, startz); // Index 8
	VB.addTexcoord0(0, 1);
	VB.addVertex(startx, -starty, startz); // Index 9
	VB.addTexcoord0(1, 0);
	VB.addVertex(-startx, starty, startz); // Index 10
	VB.addTexcoord0(1, 1);
	VB.addVertex(-startx, -starty, startz); // Index 11
	//hinten
	VB.addNormal(0, 0, -1);
	VB.addTexcoord0(0, 0);
	VB.addVertex(startx, starty, -startz); // Index 12
	VB.addTexcoord0(0, 1);
	VB.addVertex(startx, -starty, -startz); // Index 13
	VB.addTexcoord0(1, 0);
	VB.addVertex(-startx, starty, -startz); // Index 14
	VB.addTexcoord0(1, 1);
	VB.addVertex(-startx, -starty, -startz); // Index 15
	//links
	VB.addNormal(-1, 0, 0);
	VB.addTexcoord0(0, 0);
	VB.addVertex(-startx, starty, startz); // Index 16
	VB.addTexcoord0(0, 1);
	VB.addVertex(-startx, -starty, startz); // Index 17
	VB.addTexcoord0(1, 0);
	VB.addVertex(-startx, starty, -startz); // Index 18
	VB.addTexcoord0(1, 1);
	VB.addVertex(-startx, -starty, -startz); // Index 19
	//rechts
	VB.addNormal(1, 0, 0);
	VB.addTexcoord0(0, 0);
	VB.addVertex(startx, starty, startz); // Index 20
	VB.addTexcoord0(0, 1);
	VB.addVertex(startx, -starty, startz); // Index 21
	VB.addTexcoord0(1, 0);
	VB.addVertex(startx, starty, -startz); // Index 22
	VB.addTexcoord0(1, 1);
	VB.addVertex(startx, -starty, -startz); // Index 23

	VB.end();

	// 2. setup index buffer

	IB.begin();

	//oben
	IB.addIndex(1);
	IB.addIndex(2);
	IB.addIndex(3);
	IB.addIndex(1);
	IB.addIndex(0);
	IB.addIndex(2);
	//unten
	IB.addIndex(7);
	IB.addIndex(6);
	IB.addIndex(5);
	IB.addIndex(6);
	IB.addIndex(4);
	IB.addIndex(5);
	//vorne
	IB.addIndex(9);
	IB.addIndex(10);
	IB.addIndex(11);
	IB.addIndex(9);
	IB.addIndex(8);
	IB.addIndex(10);
	//hinten
	IB.addIndex(15);
	IB.addIndex(14);
	IB.addIndex(13);
	IB.addIndex(14);
	IB.addIndex(12);
	IB.addIndex(13);
	//links
	IB.addIndex(17);
	IB.addIndex(18);
	IB.addIndex(19);
	IB.addIndex(17);
	IB.addIndex(16);
	IB.addIndex(18);
	//rechts
	IB.addIndex(23);
	IB.addIndex(22);
	IB.addIndex(21);
	IB.addIndex(22);
	IB.addIndex(20);
	IB.addIndex(21);

	IB.end();
}


void TriangleBoxModel::draw(const BaseCamera& Cam)
{
	BaseModel::draw(Cam);

	VB.activate();
	IB.activate();
	glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);
	IB.deactivate();
	VB.deactivate();
}


