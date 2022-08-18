//
//  BaseModel.cpp
//  ogl4
//
//  Created by Philipp Lensing on 19.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "BaseModel.h"

BaseModel::BaseModel() : pShader(NULL), DeleteShader(false), ShadowCaster(true), size(1)
{
    Transform.identity();
}

BaseModel::BaseModel(Vector& postion, float size) : BaseModel()
{
    this->size = size;
    Matrix scaleMat, posMat;
    scaleMat.scale(size);
    posMat.translation(postion);

    transform(posMat * scaleMat);
}

BaseModel::~BaseModel()
{
    if(DeleteShader && pShader)
        delete pShader;
    DeleteShader = false;
    pShader = NULL;
}

void BaseModel::shader( BaseShader* shader, bool deleteOnDestruction )
{
    pShader = shader;
    DeleteShader = deleteOnDestruction;
}

void BaseModel::draw(const BaseCamera& Cam)
{
    if(!pShader) {
        std::cout << "BaseModel::draw() no shader found" << std::endl;
        return;
    }
    
    pShader->modelTransform(transform());
    pShader->activate(Cam);
    
}

