#pragma once
#include "circle.h"
#include "raylib.h"
#include <vector>
#include "raymath.h"
#include <cmath>
#include <iostream>
#include "src/common.h"
#include "terrain.h"
#include "sneaker.cpp"
#include<random>

class Animal{
    public:
        Animal(Terrain &terrain);
        int circleNumber;
        float maxAngle;
        void Draw();
        void Update();
        Terrain terrain;
        Sneaker sneaker;
        std::vector<Circle> circles;
        static Vector3 targetplayer;
    private:
        void HandleControls();
        void Chained();
        float rotationAngle;
        std::vector<Vector3> positions;
        Circle circle;
        float chainLength;
        float radius;
        Color color;
        float speed;
        void MaxAngle();
        bool draw;
        void HeadRestriction();
        float maxHeadAngle;
        void RandomAnimal();
};