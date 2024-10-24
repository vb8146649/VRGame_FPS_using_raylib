#include "player.h"

Player::Player()
{
    playerSpeed=0.1;
    position={0,8,0};
}

void Player::Draw()
{
    gun.Draw();
}

void Player::Update(Camera& camera)
{
    Vector3 forward = Vector3Normalize(Vector3Subtract((Vector3){camera.target.x,0,camera.target.z}, (Vector3){camera.position.x,0,camera.position.z}));
    forward2 = Vector3Normalize(Vector3Subtract(camera.target,camera.position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward,{0,1,0}));
    if (IsKeyDown(KEY_W)) position = Vector3Add(position, Vector3Scale(forward, playerSpeed));
    if (IsKeyDown(KEY_S)) position = Vector3Subtract(position, Vector3Scale(forward, playerSpeed));
    if (IsKeyDown(KEY_D)) position = Vector3Add(position, Vector3Scale(right, playerSpeed));
    if (IsKeyDown(KEY_A)) position = Vector3Subtract(position, Vector3Scale(right, playerSpeed));
    camera.position=position;
    camera.target = Vector3Add(camera.position, forward2);
    HandleControls();  
    gun.Update(camera);
}

void Player::HandleControls()
{
    if (IsKeyDown(KEY_SPACE) && gun.recoiltrue==false) {
        gun.recoiltime=GetTime();
        gun.recoiltrue=true;
        // cout<<"Entering reloading"<<endl;
    }
}