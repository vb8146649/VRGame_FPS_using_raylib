#pragma once
#include <raylib.h>
#include <iostream>

class Circle{
    public:
        Circle(float radius,float chainLength, Vector3 position);
        void Draw();
        static Color color;
        Vector3 position;
        Vector3 head;
        float radius;
        float chainLength;
    private:
};