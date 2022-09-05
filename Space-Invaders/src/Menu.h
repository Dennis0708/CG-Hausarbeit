#pragma once

#include "TriangleBoxModel.h"
#include "GameState.h"
#include "BaseShader.h"
#include "PhongShader.h"
#include "Drawable.h"

class Menu : public TriangleBoxModel, public Drawable
{
    enum class MenuState
    {
        RESUME, RESET, EXIT
    };
    MenuState state;
    TriangleBoxModel* resume;
    TriangleBoxModel* reset;
    TriangleBoxModel* exit;
    TriangleBoxModel* current;
    float height;
    float lastInput;
public:
    Menu(float Width = 1, float Height = 1, float Depth = 1);
    Menu(const BaseModel &toCopy, float Width = 1, float Height = 1, float Depth = 1);
    virtual ~Menu();
    void init(float Width, float Height, float Depth);
    void up();
    void down();
    GameState enter();
    void update(float dtime);
    void show();
    void hide();
    void transform(const Matrix& m) override;
    void draw(const BaseCamera& Cam) override;
};