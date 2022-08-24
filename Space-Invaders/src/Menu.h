#pragma once

#include "TriangleBoxModel.h"
#include "GameState.h"
#include "BaseShader.h"
#include "PhongShader.h"

class Menu : public TriangleBoxModel
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
public:
    Menu(float Width = 1, float Height = 1, float Depth = 1);
    void up();
    void down();
    GameState enter();
    void update();
    void show();
    void hide();
    void transform(const Matrix& m) override;
    void draw(const BaseCamera& Cam) override;
};