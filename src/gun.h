#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>
#include "bullet.h"
#include "raymath.h"
#include <iostream>
using namespace std;

class Gun{
    public:
        Gun();
        void Draw();
        void Update(Camera camera);
        Vector3 position;
        bool recoiltrue;
        float recoiltime;
        void Gunfire(Vector3 forward);
        void DrawBullets();
        void UpdateBullets();
        void Reload();
        vector<Bullet> bullets;
    private:
        float reloadlastTime;
        float RPS;
        float damage;
        int ammoSize;
        Model gun;
        void Recoil(Camera camera);
        void GuninHandPos(Camera camera);
        float recoil;
};