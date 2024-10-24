#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "player.h"
#include "terrain.h"
#include "animal.h"
#define FLT_MAX     340282346638528859811704183484516925440.0f
#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION        330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION        100
#endif

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    // NOTE: screenWidth/screenHeight should match VR device aspect ratio
    InitWindow(screenWidth, screenHeight, "raylib [core] example - vr simulator");
    float time= GetTime();
    // VR device parameters definition
    VrDeviceInfo device;
    device.hResolution = 2160;                 // Horizontal resolution in pixels
    device.vResolution = 1200;                 // Vertical resolution in pixels
    device.hScreenSize = 0.133793f;            // Horizontal size in meters
    device.vScreenSize = 0.0669f;              // Vertical size in meters
    device.eyeToScreenDistance = 0.041f;       // Distance between eye and display in meters
    device.lensSeparationDistance = 0.07f;     // Lens separation distance in meters
    device.interpupillaryDistance = 0.07f;     // IPD (distance between pupils) in meters

    // NOTE: CV1 uses fresnel-hybrid-asymmetric lenses with specific compute shaders
    // Following parameters are just an approximation to CV1 distortion stereo rendering
    device.lensDistortionValues[0] = 1.0f;     // Lens distortion constant parameter 0
    device.lensDistortionValues[1] = 0.22f;    // Lens distortion constant parameter 1
    device.lensDistortionValues[2] = 0.24f;    // Lens distortion constant parameter 2
    device.lensDistortionValues[3] = 0.0f;     // Lens distortion constant parameter 3
    device.chromaAbCorrection[0] = 0.996f;     // Chromatic aberration correction parameter 0
    device.chromaAbCorrection[1] = -0.004f;    // Chromatic aberration correction parameter 1
    device.chromaAbCorrection[2] = 1.014f;     // Chromatic aberration correction parameter 2
    device.chromaAbCorrection[3] = 0.0f;       // Chromatic aberration correction parameter 3

    // Load VR stereo config for VR device parameteres (Oculus Rift CV1 parameters)
    VrStereoConfig config = LoadVrStereoConfig(device);

    // Distortion shader (uses device lens distortion and chroma)
    Shader distortion = LoadShader(0, TextFormat("raylib/examples/core/resources/distortion%i.fs", GLSL_VERSION));

    // Update distortion shader with lens and distortion-scale parameters
    SetShaderValue(distortion, GetShaderLocation(distortion, "leftLensCenter"),
                   config.leftLensCenter, SHADER_UNIFORM_VEC2);
    SetShaderValue(distortion, GetShaderLocation(distortion, "rightLensCenter"),
                   config.rightLensCenter, SHADER_UNIFORM_VEC2);
    SetShaderValue(distortion, GetShaderLocation(distortion, "leftScreenCenter"),
                   config.leftScreenCenter, SHADER_UNIFORM_VEC2);
    SetShaderValue(distortion, GetShaderLocation(distortion, "rightScreenCenter"),
                   config.rightScreenCenter, SHADER_UNIFORM_VEC2);

    SetShaderValue(distortion, GetShaderLocation(distortion, "scale"),
                   config.scale, SHADER_UNIFORM_VEC2);
    SetShaderValue(distortion, GetShaderLocation(distortion, "scaleIn"),
                   config.scaleIn, SHADER_UNIFORM_VEC2);
    SetShaderValue(distortion, GetShaderLocation(distortion, "deviceWarpParam"),
                   device.lensDistortionValues, SHADER_UNIFORM_VEC4);
    SetShaderValue(distortion, GetShaderLocation(distortion, "chromaAbParam"),
                   device.chromaAbCorrection, SHADER_UNIFORM_VEC4);

    // Initialize framebuffer for stereo rendering
    // NOTE: Screen size should match HMD aspect ratio
    RenderTexture2D target = LoadRenderTexture(device.hResolution, device.vResolution);

    // The target's height is flipped (in the source Rectangle), due to OpenGL reasons
    Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, -(float)target.texture.height };
    Rectangle destRec = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };


    // Player
    Player player;
    Terrain terrain;
    vector<Animal> animals;
    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = player.position;    // Camera position
    camera.position.y=player.position.y+1;
    camera.target = (Vector3){0,player.position.y,1};      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type


    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(90);                   // Set our game to run at 90 frames-per-second



    //--------------------------------------------------------------------------------------

    // Main game loop
    BeginTextureMode(target);
        ClearBackground(BLACK);
        BeginVrStereoMode(config);
            BeginMode3D(camera);
            EndMode3D();
        EndVrStereoMode();
    EndTextureMode();
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FIRST_PERSON);
        //----------------------------------------------------------------------------------
        terrain.Update(player);
        player.HandleControls();
        player.Update(camera);
        if(GetTime()-time>10){
            Animal animal(terrain);
            animals.push_back(animal);
            time =GetTime();
        }
        if(animals.size()>0){
            animals[0].targetplayer=player.position;
        }
        for(int i=0;i<animals.size();i++){
            animals[i].Update();
            bool tag=false;
            for(int j=0;j<animals[i].circles.size();j++){
                for(int z=0;z<player.gun.bullets.size();z++){
                    if(CheckCollisionSpheres(player.gun.bullets[z].position, 0.01f, animals[i].circles[j].position, animals[i].circles[j].radius)){
                        tag=true;
                        break;
                    }
                }
            }
            if(tag==true){
                animals.erase(i+animals.begin());
            }
        }
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginTextureMode(target);
            ClearBackground(BLACK);
            BeginVrStereoMode(config);
                BeginMode3D(camera);
                    terrain.Draw();
                    DrawGrid(40, 1.0f);
                    player.Draw();
                    for(int i=0;i<animals.size();i++){
                        animals[i].Draw();
                    }
                EndMode3D();
            EndVrStereoMode();
        EndTextureMode();
        
        BeginDrawing();
            ClearBackground(BLACK);
            BeginShaderMode(distortion);
                DrawTexturePro(target.texture, sourceRec, destRec, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
            EndShaderMode();
            DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadVrStereoConfig(config);   // Unload stereo config

    UnloadRenderTexture(target);    // Unload stereo render fbo
    UnloadShader(distortion);       // Unload distortion shader

    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
