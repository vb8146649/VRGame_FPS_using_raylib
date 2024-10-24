#pragma once
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include "raylib/src/rlgl.h"
using namespace std;

class Bullet{
    public:
        Bullet(float speed,float height,float size,Vector3 position,Vector3 forward);
        void Draw();
        void Update();
        Vector3 position;
    private:
        float speed;
        float size;
        float height;
        Vector3 forward;
};


