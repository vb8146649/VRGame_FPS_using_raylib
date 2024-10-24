#include "bullet.h"

Bullet::Bullet(float speed,float height, float size, Vector3 position, Vector3 forward)
{
    this->position=position;
    this->size=size;
    this->height=height;
    this->forward=forward;
    this->speed=speed;
}

void Bullet::Draw()
{
    Vector3 direction = Vector3Normalize(forward);

    // Calculate the rotation axis (cross product between default forward (0, 0, 1) and the bullet's direction)
    Vector3 defaultForward = { 0.0f, 1.0f, 0.0f };
    Vector3 rotationAxis = Vector3CrossProduct(defaultForward, direction);

    // Calculate the angle between the default forward and the bullet's direction
    float angle = acosf(Vector3DotProduct(defaultForward, direction));

    // Apply transformations (rotation) to align the bullet with its direction
    rlPushMatrix();  // Push the current transformation matrix

    // Translate to the bullet's position
    rlTranslatef(position.x, position.y, position.z);

    // Rotate the bullet to face the correct direction
    rlRotatef(RAD2DEG * angle, rotationAxis.x, rotationAxis.y, rotationAxis.z);

    // Draw the cylinder after applying transformations
    DrawCylinder(Vector3{0,0,0}, size, size, height, 10, WHITE);

    rlPopMatrix();
    // cout<<"Drawing bullet"<<endl;
}

void Bullet::Update()
{
    position= Vector3Add(Vector3Scale(forward,speed),position);
    cout<<position.x<<" "<<position.y<<" "<<position.z<<endl;
}
