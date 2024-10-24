
#include "animal.h"

Vector3 Animal::targetplayer=(Vector3){0,0,0};

Animal::Animal(Terrain &terrain) : circle(radius, chainLength,Vector3{0,0,0}) {
    radius = 0.2f;
    chainLength = 0.1f;
    rotationAngle = 0.1f;
    color = WHITE;
    speed = 0.1;
    maxAngle = PI / 2;
    maxHeadAngle = PI / 2;
    draw = true;
    this->terrain=terrain;
    RandomAnimal();
}

void Animal::RandomAnimal(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0,16);
    int random_numberx=dis(gen);
    int random_numberz=dis(gen);
    for(int i=0;i<sneaker.size;i++){
        circle = Circle(sneaker.radiusarray[i], sneaker.chainlengtharray[i],Vector3{(float)random_numberx,10,(float)random_numberz});
        circles.push_back(circle);
    }
}

void Animal::Draw() {
    Vector3 templ = circles[0].head;
    Vector3 tempr = circles[0].head;

    for (size_t i = 0; i < circles.size(); ++i) {
        if (draw) {
            circles[i].Draw();
        }

        Vector3 dir = Vector3Subtract(circles[i].head, circles[i].position);
        float theta = atan2f(dir.z, dir.x);  // Swapped y with z

        DrawLine3D(templ, 
            Vector3Add(circles[i].position, 
                       Vector3{radius * cosf(theta + PI / 2), 0, 
                               radius * sinf(theta + PI / 2)}), 
            RED);

        DrawLine3D(tempr, 
            Vector3Add(circles[i].position, 
                       Vector3{radius * cosf(theta - PI / 2), 0, 
                               radius * sinf(theta - PI / 2)}), 
            RED);

        templ = Vector3Add(circles[i].position, 
                           Vector3{radius * cosf(theta + PI / 2), 0, 
                                   radius * sinf(theta + PI / 2)});

        tempr = Vector3Add(circles[i].position, 
                           Vector3{radius * cosf(theta - PI / 2), 0, 
                                   radius * sinf(theta - PI / 2)});

        if (i == circles.size() - 1) {
            DrawLine3D(templ, 
                Vector3Add(circles[i].position, 
                           Vector3{radius * cosf(theta + PI), 0, 
                                   radius * sinf(theta + PI)}), 
                RED);

            DrawLine3D(tempr, 
                Vector3Add(circles[i].position, 
                           Vector3{radius * cosf(theta - PI), 0, 
                                   radius * sinf(theta - PI)}), 
                RED);
        }
    }
}

void Animal::Update() {
    Circle::color = color;
    HandleControls();
    HeadRestriction();
    MaxAngle();
    Chained();
}

void Animal::HandleControls() { 
    Vector3 dir = Vector3Subtract(targetplayer, circles[0].position);
    float theta = atan2f(dir.z, dir.x);  // Calculate angle towards target (z and x)

    // Adjust the animal's head to face the target
    circles[0].head = Vector3Add(circles[0].position, 
                                 Vector3{radius * cosf(theta), 0, 
                                         radius * sinf(theta)});

    // Move the animal towards the target
    Vector3 movement = Vector3Scale(Vector3Normalize(dir), speed);
    circles[0].position = Vector3Add(circles[0].position, movement);
}

void Animal::Chained() {
    for (auto it = circles.begin(); it != circles.end(); ++it) {
        if(it>= circles.begin()+1){
            Vector3 direction = Vector3Normalize(
                Vector3Subtract(it->position, (it - 1)->position));
            it->position = Vector3Add((it - 1)->position, 
                                    Vector3Scale(direction, it->chainLength));
        }
        it->position.y = terrain.GetHeight(it->position.x, it->position.z)+it->radius;
    }
}

void Animal::MaxAngle() {
    for (size_t i = 1; i < circles.size() - 1; ++i) {
        Vector3 v1 = Vector3Subtract(circles[i - 1].position, circles[i].position);
        Vector3 v2 = Vector3Subtract(circles[i + 1].position, circles[i].position);

        Vector3 crossProduct = Vector3CrossProduct(v1, v2);
        float theta = asinf(Vector3Length(crossProduct) / 
                            (Vector3Length(v1) * Vector3Length(v2)));

        if (fabsf(theta) > maxAngle) {
            int sign = (crossProduct.y < 0) ? -1 : 1;  // Swapped z with y
            Vector3 rotatedVec = Vector3RotateByAxisAngle(
                v2,                      // Vector to be rotated
                Vector3{0, 1, 0},        // Axis of rotation (swapped y with z)
                sign * (fabsf(theta) - maxAngle)  // Angle of rotation
            );

            circles[i + 1].position = Vector3Add(circles[i].position, 
                                                 Vector3Scale(Vector3Normalize(rotatedVec), circles[i+1].chainLength));
        }
    }
}

void Animal::HeadRestriction() {
    if (circles.size() >= 3) {
        Vector3 crossProduct = Vector3CrossProduct(
            Vector3Subtract(circles[0].head, circles[0].position),
            Vector3Subtract(circles[1].position, circles[0].position));

        float theta = asinf(Vector3Length(crossProduct) / 
                            (radius * chainLength));

        if (fabsf(theta) > maxHeadAngle) {
            int sign = (crossProduct.y < 0) ? -1 : 1;  // Swapped z with y
            Vector3 newVector = Vector3RotateByAxisAngle(
                Vector3Subtract(circles[0].head, circles[0].position),  // Vector to rotate
                Vector3{0, 1, 0},                                       // Axis of rotation
                -sign * (fabsf(theta) - maxHeadAngle)                    // Rotation angle
            );

            circles[0].head = Vector3Add(circles[0].position, 
                                         Vector3Scale(Vector3Normalize(newVector), radius));
        }
    }
}

