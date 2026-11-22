#include "Win.h"

Win::Win()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

Win::Win(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

Win::~Win() { shutdown(); }

void Win::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    mWinSound = LoadSound("assets/win.wav");
    PlaySound(mWinSound);
}

void Win::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = MENU;
}

void Win::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    const char *title = "GAME WON";
    const char *message = "Press ENTER to return to the menu";

    int titleWidth = MeasureText(title, 60);
    int messageWidth = MeasureText(message, 25);

    DrawText(
        title,
        static_cast<int>(mOrigin.x - titleWidth / 2.0f),
        static_cast<int>(mOrigin.y - 60.0f),
        60,
        GREEN
    );

    DrawText(
        message,
        static_cast<int>(mOrigin.x - messageWidth / 2.0f),
        static_cast<int>(mOrigin.y + 30.0f),
        25,
        WHITE
    );
}

void Win::shutdown()
{
    UnloadSound(mWinSound);
}