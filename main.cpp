/**

* Author: Enes Lazim

* Assignment: Pong Clone

* Date due: [10/05/2026]

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

#include "CS3113/cs3113.h"
#include <cmath>

// Global Constants
constexpr int SCREEN_WIDTH  = 1600,
              SCREEN_HEIGHT = 900,
              FPS           = 60;

float gPulseTime = 0.0f;
float gRotation = 0.0f;
float gWalkSpeed = 100.0f;
float gWalkDirection = 1.0f;
float gWalkTime = 0.0f;
float MAX_AMP = 20.0f;
float gPreviousTicks = 0.0f;

constexpr char STICKMAN_FP[] = "assets/stickman.png";
constexpr char BACKPACK_FP[] = "assets/backpack.png";
constexpr char LASAGNA_FP[] = "assets/lasagna.png";

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE = { 200.0f, 200.0f };
constexpr Vector2 STICKMAN_START_POSITION = {80.0f, SCREEN_HEIGHT/2};
constexpr Vector2 LASAGNA_START_POSITION = {ORIGIN.x, ORIGIN.y};
constexpr float WALK_DURATION = 10.0f;

Vector2 gPosition = STICKMAN_START_POSITION;
Vector2 backpackPosition = {0.0f, 0.0f}; // Placeholder value as this will be continously updated in update() relative to stickman
Vector2 gScale = BASE_SIZE;
Vector2 lasagnaPosition = LASAGNA_START_POSITION;

// Textures
Texture2D gTexture;
Texture2D backpackTexture;
Texture2D lasagnaTexture;

// Global Variables
Color gBackgroundColor = ColorFromHex("#B2AAC6");
AppStatus gAppStatus = RUNNING;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures & Delta Time");

    gTexture = LoadTexture(STICKMAN_FP);
    backpackTexture = LoadTexture(BACKPACK_FP);
    lasagnaTexture = LoadTexture(LASAGNA_FP);
    

    SetTargetFPS(FPS);
}

void processInput()
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    // Setting up delta time
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    gPulseTime += deltaTime;

    gPosition.x += gWalkSpeed * deltaTime * gWalkDirection;
    gPosition.y = SCREEN_HEIGHT/2 + 40.0f * sinf(gPulseTime*3.0f);
    gWalkTime += deltaTime;
    gRotation = 10.0f * sinf(gPulseTime * 5.0f);

    if (gWalkTime >=  WALK_DURATION) // Change directions if walk duration is reached
    {
        gWalkDirection *= -1.0f;
        gWalkTime = 0.0f;
    }

    gScale = Vector2{ // Based on the 
        BASE_SIZE.x + MAX_AMP * sinf(gPulseTime),
        BASE_SIZE.y + MAX_AMP * tanf(gPulseTime) // Made this tangent as I love getting it giant for no reason!
    };

    // Backpack position relative to stickman
    backpackPosition.x = gPosition.x - (100.0f * gWalkDirection * cosf(gPulseTime*3.0f));
    backpackPosition.y = gPosition.y + 50.0f * (sinf(gPulseTime * 6.0f));

    // Lasagna doing weird movements only
    lasagnaPosition.x = ORIGIN.x + 250.0f * cosf(gPulseTime*0.67f) + 67.0f * sinf(gPulseTime*0.76);

    // Background color changing
    float colors = (sinf(gPulseTime * 0.67f) + 1.0f) / 2.0f;

    gBackgroundColor = {
        static_cast<unsigned char>(100 + 67 * colors),
        static_cast<unsigned char>(167 + 76 * colors),
        static_cast<unsigned char>(67 + 167 * colors),
        255
    };
}

void render()
{
    BeginDrawing();

    ClearBackground(gBackgroundColor);

    // Stickman
    Rectangle textureArea = {
        0.0f, 0.0f,
        static_cast<float>(gTexture.width),
        static_cast<float>(gTexture.height)
    };

    Rectangle destinationArea = {
        gPosition.x,
        gPosition.y,

        static_cast<float>(gScale.x),
        static_cast<float>(gScale.y)
    };

    Vector2 originOffset = {
        static_cast<float>(gScale.x) / 2.0f,
        static_cast<float>(gScale.y) / 2.0f
    };

    DrawTexturePro(
        gTexture,
        textureArea,
        destinationArea,
        originOffset,
        gRotation,
        WHITE
    );

    // Backpack
        Rectangle backpackTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(backpackTexture.width),
        static_cast<float>(backpackTexture.height)
    };

    Rectangle backpackDestinationArea = {
        backpackPosition.x,
        backpackPosition.y,
        100.0f,
        100.0f
    };

    Vector2 backpackOriginOffset = {
        static_cast<float>(gScale.x) / 2.0f,
        static_cast<float>(gScale.y) / 2.0f
    };

    DrawTexturePro(
        backpackTexture,
        backpackTextureArea,
        backpackDestinationArea,
        backpackOriginOffset,
        0.0f,
        WHITE
    );

    // Lasagna
            Rectangle lasagnaTextureArea = {
        0.0f,
        0.0f,
        static_cast<float>(lasagnaTexture.width),
        static_cast<float>(lasagnaTexture.height)
    };

    Rectangle lasagnaDestinationArea = {
        lasagnaPosition.x,
        lasagnaPosition.y,
        100.0f,
        100.0f
    };

    Vector2 lasagnaOriginOffset = {
        static_cast<float>(gScale.x) / 2.0f,
        static_cast<float>(gScale.y) / 2.0f
    };

    DrawTexturePro(
        lasagnaTexture,
        lasagnaTextureArea,
        lasagnaDestinationArea,
        lasagnaOriginOffset,
        0.0f,
        WHITE
    );

    EndDrawing();
}

void shutdown()
{
    CloseWindow();
    UnloadTexture(gTexture);
    UnloadTexture(backpackTexture);
    UnloadTexture(lasagnaTexture);
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}