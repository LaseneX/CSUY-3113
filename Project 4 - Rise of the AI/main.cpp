#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/Menu.h"
#include "CS3113/Loss.h"
#include "CS3113/Win.h"

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
extern int gLives;

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus       = RUNNING;
float     gPreviousTicks   = 0.0f,
          gTimeAccumulator = 0.0f;
int       gLives           = INITIAL_LIVES;

Camera2D gCamera = {};

Texture2D gHeartTexture;

Scene                     *gCurrentScene = nullptr;
std::map<SceneID, Scene *> gLevels       = {};

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{
    if (gCurrentScene != nullptr)
    {
        gCurrentScene->shutdown();
        gCurrentScene->setNextSceneID(NO_SCENE);
    }

    gCurrentScene = scene;
    gCurrentScene->initialise();

    if (gCurrentScene->getState().player != nullptr) { gCamera.target = gCurrentScene->getState().player->getPosition(); }
    else { gCamera.target = ORIGIN; }
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scenes");
    InitAudioDevice();

    gLevels[MENU]    = new Menu(ORIGIN, "#202030");
    gLevels[LEVEL_A] = new LevelA(ORIGIN, "#000000");
    gLevels[LEVEL_B] = new LevelB(ORIGIN, "#FFFFFF");
    gLevels[LEVEL_C] = new LevelC(ORIGIN, "#808080");
    gLevels[LOSE]    = new Loss(ORIGIN, "#202030");
    gLevels[WIN]     = new Win(ORIGIN, "#202030");

    gHeartTexture = LoadTexture("assets/heart.png");

    switchToScene(gLevels[MENU]);

    gCamera.offset   = ORIGIN;
    gCamera.rotation = 0.0f;
    gCamera.zoom     = 1.0f;

    SetTargetFPS(FPS);
}

void processInput()
{
    Entity *player = gCurrentScene->getState().player;

    if (player == nullptr)
    {
        if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
        return;
    }

    if (IsKeyPressed(KEY_ONE))
    {
        switchToScene(gLevels[LEVEL_A]);
        return;
    }

    if (IsKeyPressed(KEY_TWO))
    {
        switchToScene(gLevels[LEVEL_B]);
        return;
    }

    if (IsKeyPressed(KEY_THREE))
    {
        switchToScene(gLevels[LEVEL_C]);
        return;
    }
    
    gCurrentScene->getState().player->resetMovement();

    if      (IsKeyDown(KEY_A)) gCurrentScene->getState().player->moveLeft();
    else if (IsKeyDown(KEY_D)) gCurrentScene->getState().player->moveRight();

    if (IsKeyPressed(KEY_W) &&
        gCurrentScene->getState().player->isCollidingBottom())
    {
        gCurrentScene->getState().player->jump();
        PlaySound(gCurrentScene->getState().jumpSound);
    }

    if (GetLength(gCurrentScene->getState().player->getMovement()) > 1.0f) { gCurrentScene->getState().player->normaliseMovement(); }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;

}

void update()
{
    // Delta time
    float ticks     = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;

        Entity *player = gCurrentScene->getState().player;

        if (player != nullptr)
        {
            Vector2 currentPlayerPosition = {
                gCurrentScene->getState().player->getPosition().x, gCurrentScene->getState().player->getPosition().y
            };
        
            panCamera(&gCamera, &currentPlayerPosition);
        }
    }

    gTimeAccumulator = deltaTime;
}

void render()
{
    BeginDrawing();
    BeginMode2D(gCamera);

    gCurrentScene->render();

    EndMode2D();
    if (gCurrentScene != gLevels[WIN] && gCurrentScene != gLevels[LOSE] && gCurrentScene != gLevels[MENU])
    {
        Texture2D heart = gHeartTexture;

        for (int i = 0; i < gLives; i++)
        {
            DrawTexturePro(
                gHeartTexture,
                { 0.0f, 0.0f, (float)heart.width, (float)heart.height },
                {
                    SCREEN_WIDTH - 20.0f - 32
                        - i * (42),
                    20.0f,
                    32,
                    32
                },
                { 0.0f, 0.0f },
                0.0f,
                WHITE
                );
            }
    }
    EndDrawing();
}

void shutdown()
{
    for (std::pair<const SceneID, Scene *> &entry : gLevels)
        delete entry.second;

    gLevels.clear();
    UnloadTexture(gHeartTexture);

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        SceneID nextSceneID = gCurrentScene->getNextSceneID();
        if (nextSceneID != NO_SCENE) { switchToScene(gLevels[nextSceneID]); }

        render();
    }

    shutdown();

    return 0;
}
