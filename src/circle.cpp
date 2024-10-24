#include "circle.h"

Color Circle::color=RED;

Circle::Circle(float radius, float chainLength, Vector3 position)
{
    this->chainLength=chainLength;
    this->radius=radius;
    this->position=position;
    head={0,-1*this->radius,0};
}

void Circle::Draw()
{
    DrawSphere(position,radius,color);
}

