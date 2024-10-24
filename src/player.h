#pragma once
#include "raylib.h"
#include <src/common.h>
#include "raymath.h"
#include "gun.h"
#include<iostream>
using namespace std;

class Player{
    public:
        Player();
        void Draw();
        void Update(Camera &camera);
        void HandleControls();
        Vector3 position;
        Vector3 forward2;
        Gun gun;
    private:
        float playerSpeed;
        void Jump();
        void MoveLeft();
        void MoveRight();
        void MoveForward();
        void MoveBackward();
        
};