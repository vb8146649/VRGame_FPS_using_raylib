#include "gun.h"

Gun::Gun()
{
    gun=LoadModel("src/deserteagle.glb");
    recoil=0.0;
    recoiltime=0;
    recoiltrue=false;
    ammoSize=10;
}


void Gun::Draw()
{
    DrawModel(gun,position,0.5,WHITE);
    DrawBullets();
}

void Gun::Update(Camera camera)
{
    GuninHandPos(camera);
    UpdateBullets();
    Recoil(camera);
}



void Gun::Gunfire(Vector3 forward)
{

    Bullet bullet(0.05f,0.05f,0.01f,Vector3Subtract(Vector3Add(position,Vector3Scale(Vector3Normalize(forward),0.1)),(Vector3){0,0.06,0}),forward);
    bullets.push_back(bullet);
}

void Gun::DrawBullets()
{
    for(int i=0;i<bullets.size();i++){
        bullets[i].Draw();
    }
}

void Gun::UpdateBullets()
{
    for(int i=0;i<bullets.size();i++){
        if(Vector3Length(Vector3Subtract(bullets[i].position,position))>2){
            bullets.erase(i+bullets.begin());
            cout<<"bullets deleted"<<endl;
            i--;
        }
    }
    for(int i=0;i<bullets.size();i++){
        bullets[i].Update();
    }

}

void Gun::Recoil(Camera camera)
{
    if(recoiltrue && (GetTime()-recoiltime)<=0.15){
        Vector3 direction = Vector3Subtract(camera.target, camera.position);
        direction = Vector3Normalize(direction); 
        recoil+=0.05;
        position = Vector3Add(position,Vector3Scale(direction,recoil));
        // cout<<position.x<<" "<<position.y<<" "<<position.z<<"recoil "<<recoil<<endl;
        if(Vector3Length(Vector3Subtract(camera.position,position))>0.20){
            recoil=-(recoil);
            Gunfire(direction);
        }
    }else{
        recoiltrue=false;
        recoil = 0.0f; 
    }
}

void Gun::GuninHandPos(Camera camera)
{
    position.x = camera.position.x;
    position.z = camera.position.z;
    position.y=camera.position.y;
    // Calculate the direction from the gun's position to the camera's target
    Vector3 direction = Vector3Subtract(camera.target, camera.position);
    direction = Vector3Normalize(direction);  // Normalize the direction vector

    // Calculate yaw (horizontal rotation) and pitch (vertical rotation)
    float yaw = atan2f(direction.x, direction.z);      // Yaw is the angle around the Y-axis
    float pitch = asinf(-direction.y);                 // Pitch is the angle around the X-axis

    // Create a rotation quaternion based on yaw and pitch
    Quaternion rotationYaw = QuaternionFromAxisAngle((Vector3){0.0f, 1.0f, 0.0f}, yaw);  // Yaw rotation (Y-axis)
    Quaternion rotationPitch = QuaternionFromAxisAngle((Vector3){1.0f, 0.0f, 0.0f}, pitch);  // Pitch rotation (X-axis)

    // Combine yaw and pitch rotations into a single quaternion
    Quaternion rotation = QuaternionMultiply(rotationYaw, rotationPitch);

    // Set gun position and apply the final rotation
    gun.transform = MatrixTranslate(-0.5, 0, 1);  // Set the gun's position
    gun.transform = MatrixMultiply(gun.transform, QuaternionToMatrix(rotation));  // Apply the rotation

}
