/**

* Author: Enes Lazim

* Assignment: Lunar Lander!

* Date due: 10/30/2026

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 60;

constexpr float GRAVITY = 100.0f,
                HORIZONTAL_THRUST   = 250.0f,
                VERTICAL_THRUST     = 250.0f,
                HORIZONTAL_DRAG     = 100.0f,
                MAX_FUEL            = 100.0f,
                FUEL_USAGE          = 10.0f,
                MAX_LANDING_SPEED_X = 70.0f,
                MAX_LANDING_SPEED_Y = 70.0f,
                GROUND_Y            = 560.0f,
                PLATFORM_SPEED      = 50.0f,
                PLATFORM_MIN_X      = 250.0f,
                PLATFORM_MAX_X      = 650.0f;

constexpr Vector2 SHIP_SIZE     = { 60.0f, 80.0f },
                  SHIP_INIT_POS = { 100.0f, 100.0f },
                  MESSAGE_POS   = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f},
                  MESSAGE_SCALE = { 378.0f, 30.0f };


enum GameState
{
    PLAYING,
    LOST,
    WON
};

// Source of all the assets are shared in README.md
constexpr char SHIP_FP[] = "assets/spaceship.png"; 
constexpr char BACKGROUND_FP[] = "assets/Space Background.png";
constexpr char LOST_FP[] = "assets/missionFailed.png";
constexpr char WON_FP[] = "assets/missionSuccessful.png";

// Global Variables
AppStatus gAppStatus         = RUNNING;
GameState gGameState         = PLAYING;
float     gPreviousTicks     = 0.0f,
          gFuel              = MAX_FUEL,
          gPlatformDirection = 1.0f,
          gHitboxFactor      = 0.5f; // I found 0.5 scaling to be most comfortable as hitbox factor

Vector2 gShipPosition     = SHIP_INIT_POS,
        gShipMovement     = { 0.0f, 0.0f},
        gShipAcceleration = { 0.0f, 0.0f },
        gShipScale        = SHIP_SIZE, // Rendering
        gHitboxScale      = {SHIP_SIZE.x * gHitboxFactor, SHIP_SIZE.y * gHitboxFactor}; // Actual hitbox


// Platforms
Rectangle gSafe = { 350.0f, 470.0f, 180.0f, 20.0f };

// Textures
Texture2D gShipTexture;
Texture2D gBackgroundTexture;
Texture2D gWinMessageTexture;
Texture2D gLostMessageTexture;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA, const Vector2 *positionB, const Vector2 *scaleB);

// Function Definitions

/**
 * @brief Checks for a square collision between 2 Rectangle objects.
 *
 * @param positionA The position of the first object
 * @param scaleA The scale of the first object
 * @param positionB The position of the second object
 * @param scaleB The scale of the second object
 * @return true if a collision is detected,
 * @return false if a collision is not detected
 */
bool isColliding(const Vector2 *positionA, const Vector2 *scaleA,
                 const Vector2 *positionB, const Vector2 *scaleB)
{
    float xDistance = fabsf(positionA->x - positionB->x) -
                      ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabsf(positionA->y - positionB->y) -
                      ((scaleA->y + scaleB->y) / 2.0f);

    return xDistance < 0.0f && yDistance < 0.0f;
}

void renderObject(const Texture2D *texture, const Vector2 *position,
                  const Vector2 *scale)
{
    // Whole texture (UV coordinates)
    Rectangle textureArea = {
        // top-left corner
        0.0f, 0.0f,

        // bottom-right corner (of texture)
        static_cast<float>(texture->width),
        static_cast<float>(texture->height)
    };

    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        *texture,
        textureArea, destinationArea, originOffset, 0, WHITE
    );
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
        "Lunar Lander");

    gShipTexture        = LoadTexture(SHIP_FP);
    gBackgroundTexture  = LoadTexture(BACKGROUND_FP);
    gWinMessageTexture  = LoadTexture(WON_FP);
    gLostMessageTexture = LoadTexture(LOST_FP);


    SetTargetFPS(FPS);
}

void processInput()
{

    // to close the game
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    // Delta Time
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    if (gGameState != PLAYING) return; // If you lose/win, the loop ends

    // Platform
    gSafe.x += PLATFORM_SPEED * gPlatformDirection * deltaTime;
    if (gSafe.x <= PLATFORM_MIN_X)
    {
        gSafe.x = PLATFORM_MIN_X;
        gPlatformDirection = 1.0f;
    }
    else if (gSafe.x >= PLATFORM_MAX_X)
    {
        gSafe.x = PLATFORM_MAX_X;
        gPlatformDirection = -1.0f;
    }

    float thrustScale = 0.0f;
    float previousBottom = gShipPosition.y + gHitboxScale.y / 2.0f; // Variable for bottom position of y before ship moves
    float horizontalChange = 0.0f;

    // Inputs
    if (IsKeyDown(KEY_LEFT)) horizontalChange = -1.0f;
    else if (IsKeyDown(KEY_RIGHT)) horizontalChange = 1.0f;

    // Fuel Consumption
    if ((horizontalChange != 0.0f || IsKeyDown(KEY_UP)) && gFuel > 0.0f)
    {
        float fuelNeeded = FUEL_USAGE * deltaTime; 
        if (fuelNeeded > 0.0f) 
        {
            if (gFuel >= fuelNeeded) thrustScale = 1.0f;
            else thrustScale = gFuel / fuelNeeded;
            gFuel -= fuelNeeded;

            if (gFuel <= 0.0f) gFuel = 0;
        }
        else thrustScale = 1.0f;
    }

    // Acceleration
    gShipAcceleration.x = horizontalChange * HORIZONTAL_THRUST * thrustScale;
    gShipAcceleration.y = GRAVITY;

    if (IsKeyDown(KEY_UP) && thrustScale > 0.0f) gShipAcceleration.y -= VERTICAL_THRUST * thrustScale;

    // We add air friction for x Axis so ship naturally slows down too
    if (horizontalChange == 0.0f || thrustScale == 0.0f)
    {
        if (gShipMovement.x > 0.0f)
        {
            if (deltaTime > 0.0f && gShipMovement.x <= HORIZONTAL_DRAG * deltaTime) gShipAcceleration.x = -gShipMovement.x / deltaTime;
            else gShipAcceleration.x = -HORIZONTAL_DRAG;
        }
        else if (gShipMovement.x < 0.0f)
        {
            if (deltaTime > 0.0f && -gShipMovement.x <= HORIZONTAL_DRAG * deltaTime) gShipAcceleration.x = -gShipMovement.x / deltaTime;
            else gShipAcceleration.x = HORIZONTAL_DRAG;
        }
    }

    // Velocity
    gShipMovement.x += gShipAcceleration.x * deltaTime;
    gShipMovement.y += gShipAcceleration.y * deltaTime;

    // Position
    gShipPosition.x += gShipMovement.x * deltaTime;
    gShipPosition.y += gShipMovement.y * deltaTime;

    // Collision
    Vector2 safeposition = { gSafe.x + gSafe.width / 2.0f, gSafe.y + gSafe.height / 2.0f },
            safeScale    = { gSafe.width, gSafe.height };

    bool collidingSafely = isColliding(&gShipPosition, &gHitboxScale, &safeposition, &safeScale);

    // Calculate Win/Loss
    if (gShipMovement.y >= 0.0f && 
        (previousBottom <= gSafe.y && gShipPosition.y + gHitboxScale.y / 2.0f >= gSafe.y) && 
        (gShipPosition.x + gHitboxScale.x / 2.0f >= gSafe.x && gShipPosition.x - gHitboxScale.x / 2.0f <= gSafe.x + gSafe.width))
    {
        gShipPosition.y = gSafe.y - gHitboxScale.y / 2.0f;

        float relativeHorizontalSpeed = gShipMovement.x - PLATFORM_SPEED * gPlatformDirection;

        if (fabsf(relativeHorizontalSpeed) <= MAX_LANDING_SPEED_X &&
            fabsf(gShipMovement.y) <= MAX_LANDING_SPEED_Y) gGameState = WON;
        else gGameState = LOST;

        gShipMovement     = { 0.0f, 0.0f };
        gShipAcceleration = { 0.0f, 0.0f };

        return;
    }
    
    if (isColliding(&gShipPosition, &gHitboxScale, &safeposition, &safeScale))
    {
        gGameState        = LOST;
        gShipMovement     = { 0.0f, 0.0f };
        gShipAcceleration = { 0.0f, 0.0f };
        return;
    }

    // Ground
    if (gShipPosition.y + gHitboxScale.y / 2.0f >= GROUND_Y)
    {
        gShipPosition.y   = GROUND_Y - gHitboxScale.y / 2.0f;
        gGameState        = LOST;
        gShipMovement     = { 0.0f, 0.0f };
        gShipAcceleration = { 0.0f, 0.0f };
        return;
    }

    // Screen Boundaries
    if (gShipPosition.x - gHitboxScale.x / 2.0f < 0.0f || 
        gShipPosition.x + gHitboxScale.x / 2.0f > SCREEN_WIDTH || 
        gShipPosition.y - gHitboxScale.y / 2.0f < 0.0f)
    {
            gGameState        = LOST;
            gShipMovement     = { 0.0f, 0.0f };
            gShipAcceleration = { 0.0f, 0.0f };
            return;                
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(BLACK); 

    // Background
    Rectangle backgroundArea = { 0.0f, 0.0f, static_cast<float>(gBackgroundTexture.width), static_cast<float>(gBackgroundTexture.height) };
    DrawTexturePro(gBackgroundTexture, backgroundArea, {0.0f, 0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)}, {0.0f, 0.0f}, 0.0f, WHITE);

    // Ground
    DrawRectangle(0, static_cast<int>(GROUND_Y), SCREEN_WIDTH, SCREEN_HEIGHT - static_cast<int>(GROUND_Y), DARKGRAY);
    DrawLine(0, static_cast<int>(GROUND_Y), SCREEN_WIDTH, static_cast<int>(GROUND_Y), LIGHTGRAY);


    // Platform
    DrawRectangleRec(gSafe, GRAY);

    DrawRectangle(static_cast<int>(gSafe.x), static_cast<int>(gSafe.y), static_cast<int>(gSafe.width), 4, GREEN);

    // Ship
    renderObject(&gShipTexture, &gShipPosition, &gShipScale);

    // Fuel Bar Base
    DrawRectangle(20, 20, 200, 16, DARKGRAY);
    Color fuelColor = GREEN;

    // Fuel Bar changes color as the fuel decreases
    if (gFuel <= MAX_FUEL*0.5f && gFuel > MAX_FUEL*0.25f)
    {
        fuelColor = YELLOW;
    }
    else if (gFuel <= MAX_FUEL*0.25f && gFuel > 0.0f)
    {
        fuelColor = RED;
    }
    else if (gFuel <= 0.0f)
    {
        fuelColor = BLACK;
    }

    // Fuel bar gradually decreases more it is used
    DrawRectangle(20, 20, static_cast<int>(200.0f * gFuel / MAX_FUEL), 16, fuelColor);

    // Result Message
    if (gGameState == WON) renderObject(&gWinMessageTexture, &MESSAGE_POS, &MESSAGE_SCALE);
    else if (gGameState == LOST) renderObject(&gLostMessageTexture, &MESSAGE_POS, &MESSAGE_SCALE);
    
    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gShipTexture);
    UnloadTexture(gBackgroundTexture);
    UnloadTexture(gLostMessageTexture);
    UnloadTexture(gWinMessageTexture);
    CloseWindow();
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
