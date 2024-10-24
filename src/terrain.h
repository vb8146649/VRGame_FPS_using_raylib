#pragma once
#include<iostream>
#include<raylib.h>
#include<string.h>
#include<vector>
#include"player.h"
#include"raymath.h"
using namespace std;

class Terrain{
        Image image;
        Texture2D texture;
        Mesh mesh;
        Model model;
        Vector3 position;
    public:
        Terrain();
        float GetHeight(float x, float z);
        void Draw();
        void Update(Player &player);
        vector<string> maps;
        Vector3 cameratarget;
};