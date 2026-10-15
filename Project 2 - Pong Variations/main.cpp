/**

* Author: Enes Lazim

* Assignment: Pong Clone

* Date due: 10/16/2026

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 60,
              SPEED         = 200,
              MAX_BALLS     = 3,
              WINNING_SCORE = 2; // Keeping it 2 for the sake of testing

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  BALL_BASE_SIZE = { 50.0f, 50.0f },
                  BALL_INIT_POS = { ORIGIN.x, ORIGIN.y - 130.0f },
                  PONG_BASE_SIZE = { 150.0f, 250.0f },
                  PONG_LEFT_INIT_POS = { 30.0f, (SCREEN_HEIGHT - PONG_BASE_SIZE.y) / 2.0f + 100.0f},
                  PONG_RIGHT_INIT_POS = {SCREEN_WIDTH - 30.0f, (SCREEN_HEIGHT - PONG_BASE_SIZE.y) / 2.0f + 100.0f};

constexpr char BALL_FP[] = "assets/soccerBall.png";
constexpr char PONG_FP[] = "assets/pongMachine.png";
constexpr char RIGHT_FP[] = "assets/rightWin.png";
constexpr char LEFT_FP[] = "assets/leftWin.png";

// Global Variables
AppStatus gAppStatus     = RUNNING;
float     gAngle         = 0.0f,
          gPreviousTicks = 0.0f;

// Ball Variables
Vector2 gBallPositions[MAX_BALLS] = { BALL_INIT_POS, {ORIGIN.x - 100.0f, ORIGIN.y}, {ORIGIN.x + 100.0f, ORIGIN.y}};
Vector2 gBallMovements[MAX_BALLS] = { {SPEED, SPEED}, {-SPEED, SPEED}, {SPEED, -SPEED} };
Vector2 gBallScale = BALL_BASE_SIZE;

// Pong Variables
Vector2 gLeftPongPosition = PONG_LEFT_INIT_POS,
        gLeftPongMovement = { 0.0f, 0.0f };

Vector2 gRightPongPosition = PONG_RIGHT_INIT_POS,
        gRightPongMovement = { 0.0f, 0.0f };

Vector2 gPongScale = PONG_BASE_SIZE;


Texture2D gBallTexture;
Texture2D gPongTexture; // Yes they are a pong machine.
Texture2D gRightWinTexture; // README 4)
Texture2D gLeftWinTexture;

bool gSinglePlayer = false; // Boolean for single player toggle
int gBallCount = 1; // Since every match starts with one ball

// Scores
int gLeftScore  = 0;
int gRightScore = 0;

bool gGameOver = false;

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
    float xDistance = fabs(positionA->x - positionB->x) -
                      ((scaleA->x + scaleB->x) / 2.0f);
    float yDistance = fabs(positionA->y - positionB->y) -
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

    // Destination rectangle – centred on gPosition
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
        textureArea, destinationArea, originOffset,
        gAngle, WHITE
    );
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
        "User Input / Collision Detection");

    gBallTexture     = LoadTexture(BALL_FP);
    gPongTexture     = LoadTexture(PONG_FP);
    gRightWinTexture = LoadTexture(RIGHT_FP);
    gLeftWinTexture  = LoadTexture(LEFT_FP);


    SetTargetFPS(FPS);
}

void processInput()
{

    // to close the game
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
    gLeftPongMovement.y = 0.0f;
    gRightPongMovement.y = 0.0f;

    // Left Pong Input Detection if gSinglePlayer is false
    if (!gSinglePlayer)
    {
        if (IsKeyDown(KEY_W))      { gLeftPongMovement.y = -SPEED; }
        else if (IsKeyDown(KEY_S)) { gLeftPongMovement.y = SPEED; }
    }

    // Right Pong Input Detection
    if (IsKeyDown(KEY_UP))        { gRightPongMovement.y = -SPEED; }
    else if (IsKeyDown(KEY_DOWN)) { gRightPongMovement.y = SPEED; }

    // Single Player Mode activation and deactivation
    if (IsKeyPressed(KEY_T)) { gSinglePlayer = !gSinglePlayer; }

    // Input for ball count changing
    if (IsKeyPressed(KEY_ONE))        { gBallCount = 1; }
    else if (IsKeyPressed(KEY_TWO))   { gBallCount = 2; }
    else if (IsKeyPressed(KEY_THREE)) { gBallCount = 3; }
}

void update()
{
    // Delta Time
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    if (gGameOver)
    {
        return;
    }

    // Right Pong Bar Movement
    gRightPongPosition.y += gRightPongMovement.y * deltaTime;

    // Left Pong Bar Movement for AI
    if (gSinglePlayer)
    {
        int closestBall = 0;

        for (int i = 0; i < gBallCount; i++)
        {
            if (gBallPositions[i].x < gBallPositions[closestBall].x) { closestBall = i; }
        }
        if (gBallPositions[closestBall].y < gLeftPongPosition.y - 9.67f) { gLeftPongMovement.y = -SPEED*0.5; }
        else if (gBallPositions[closestBall].y > gLeftPongPosition.y + 9.67f) { gLeftPongMovement.y = SPEED*0.5; }
        else { gLeftPongMovement.y = 0.0f; }
    }
    gLeftPongPosition.y += gLeftPongMovement.y * deltaTime; // Position changing according to movement


    // Boundaries
    // Explained in README 1)
    if (gLeftPongPosition.y < 0.0f) { gLeftPongPosition.y = 0.0f; } 
    if (gLeftPongPosition.y > SCREEN_HEIGHT) { gLeftPongPosition.y = SCREEN_HEIGHT; }

    if (gRightPongPosition.y < 0.0f) { gRightPongPosition.y = 0.0f; }
    if (gRightPongPosition.y > SCREEN_HEIGHT) { gRightPongPosition.y = SCREEN_HEIGHT; }

    // More Ball Stuff
    Vector2 gPongHitbox = { gPongScale.x*0.15f, gPongScale.y }; // Explained in README 2)

    for (int i = 0; i < gBallCount; i++) // Explained in README 3)
    {
        // Ball Movement
        gBallPositions[i].x += gBallMovements[i].x * deltaTime;
        gBallPositions[i].y += gBallMovements[i].y * deltaTime;

        // Top Wall
        if (gBallPositions[i].y - gBallScale.y / 2.0f <= 0.0f)
        {
            gBallPositions[i].y = gBallScale.y / 2.0f;
            gBallMovements[i].y = -gBallMovements[i].y;
        }

        // Bottom Wall
        if (gBallPositions[i].y + gBallScale.y / 2.0f >= SCREEN_HEIGHT)
        {
            gBallPositions[i].y = SCREEN_HEIGHT - gBallScale.y / 2.0f;
            gBallMovements[i].y = -gBallMovements[i].y;
        }

        // Bar Collisions
        if (isColliding(&gBallPositions[i], &gBallScale, &gLeftPongPosition, &gPongHitbox)) // Left
        {
            if (gBallMovements[i].x < 0.0f)
            {
                gBallMovements[i].x = -gBallMovements[i].x;
            }
        }
    
        if (isColliding(&gBallPositions[i], &gBallScale, &gRightPongPosition, &gPongHitbox)) // Right
        {
            if (gBallMovements[i].x > 0.0f)
            {
                gBallMovements[i].x = -gBallMovements[i].x;
            }
        }

        // Scoring
        if (gBallPositions[i].x + gBallScale.x / 2.0f <= 0.0f) // Right Scoring
        { 
            gRightScore++; 
            gBallPositions[i] = ORIGIN;
            gBallMovements[i].x = -SPEED;

            if (gRightScore >= WINNING_SCORE)
            {
                gGameOver = true;
            }
        }

        else if (gBallPositions[i].x - gBallScale.x / 2.0f >= SCREEN_WIDTH)  // Left Scoring
        { 
            gLeftScore++; 
            gBallPositions[i] = ORIGIN;
            gBallMovements[i].x = SPEED;

            if (gLeftScore >= WINNING_SCORE)
            {
                gGameOver = true;
            }
        }

    }

}
void render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (gGameOver)
    {
        if (gRightScore >= WINNING_SCORE)
        {
            DrawTexture(gRightWinTexture, SCREEN_WIDTH / 2 - gRightWinTexture.width / 2, 
                SCREEN_HEIGHT / 2 - gRightWinTexture.height / 2, 
                WHITE );
        }
        else 
        {
            DrawTexture(gLeftWinTexture, SCREEN_WIDTH / 2 - gLeftWinTexture.width / 2, 
                SCREEN_HEIGHT / 2 - gLeftWinTexture.height / 2, 
                WHITE );
        }
    }

    // render all the balls
    if (!gGameOver)
    {
        for (int i = 0; i < gBallCount; i++) { renderObject(&gBallTexture, &gBallPositions[i], &gBallScale); }

        // render the pongs
        renderObject(&gPongTexture, &gLeftPongPosition, &gPongScale);
        renderObject(&gPongTexture, &gRightPongPosition, &gPongScale);
    }

    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gPongTexture);
    UnloadTexture(gBallTexture);
    UnloadTexture(gRightWinTexture);
    UnloadTexture(gLeftWinTexture);
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
