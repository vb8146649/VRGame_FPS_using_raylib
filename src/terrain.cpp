#include "terrain.h"

Terrain::Terrain()
{
    // image=LoadImage("/home/asus/Desktop/projects/raylib/examples/models/resources/heightmap.png");
    texture = LoadTextureFromImage(image);
    mesh = GenMeshHeightmap(image,(Vector3){32,2,32});
    model = LoadModelFromMesh(mesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture=texture;
    position={-16,0,-16};
    cameratarget=(Vector3){0,0,0};
}

void Terrain::Draw(){
    DrawModel(model,position,1.0f , RED);
}

float Terrain::GetHeight(float x, float z)
{
    Ray ray = { .position = Vector3{x,3,z}, .direction = { 0, -1, 0 } }; // Ray pointing downwards
    Matrix transform = MatrixTranslate(position.x, position.y, position.z);
    RayCollision collision = GetRayCollisionMesh(ray, mesh, transform);

    // If there is a collision with the ground (terrain), update the player's Y position
    if (collision.hit) {
        return collision.point.y; // Adjust based on character height
    }else{
        return 0;
    }
}

void Terrain::Update(Player &player)
{
    Ray ray = { .position = player.position, .direction = { 0, -1, 0 } }; // Ray pointing downwards
    Matrix transform = MatrixTranslate(position.x, position.y, position.z);
    RayCollision collision = GetRayCollisionMesh(ray, mesh, transform);

    // If there is a collision with the ground (terrain), update the player's Y position
    if (collision.hit) {
        player.position.y = collision.point.y+1 ; // Adjust based on character height
    }
}
