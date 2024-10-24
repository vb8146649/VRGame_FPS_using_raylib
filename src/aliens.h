#pragma once
#include "raylib.h"
#include<vector>
#include "sneaker.h"

class Aliens{
    public:
        Aliens();
        void Draw();
        void Update();
    private:
        Vector3 position;
        int type;
        std::vector<Sneaker> aliens;
};
