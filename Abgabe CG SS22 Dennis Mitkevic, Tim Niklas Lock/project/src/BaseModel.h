//
//  BaseModel.hpp
//  ogl4
//
//  Created by Philipp Lensing on 19.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#ifndef BaseModel_hpp
#define BaseModel_hpp

#include <stdio.h>
#include "camera.h"
#include "matrix.h"
#include "baseshader.h"
#include "Aabb.h"

class BaseModel
{
public:
    BaseModel();
    BaseModel(Vector& positon, float size=1.0f);
    BaseModel(const BaseModel& toCopy);
    virtual ~BaseModel();
    virtual void draw(const BaseCamera& Cam);
    const Matrix& transform() const { return Transform; }
    virtual void transform(const Matrix& m) { Transform = m; }
    virtual void shader( BaseShader* shader, bool deleteOnDestruction=false );
    virtual BaseShader* shader() const { return pShader; }
    virtual const AABB& boundingBox() const { return AABB::unitBox(); }
    virtual void hide();
    virtual void show(const Vector& pos);
	bool shadowCaster() const { return ShadowCaster; }
	void shadowCaster(bool sc) { ShadowCaster = sc; }
    void setSize(float size) { this->size = size; }
protected:
    Matrix Transform;
    BaseShader* pShader;
    bool DeleteShader;
	bool ShadowCaster;
    float size;
        
};


#endif  BaseModel_hpp 
