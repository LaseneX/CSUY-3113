#include "Loss.h"

Loss::Loss()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

Loss::Loss(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

Loss::~Loss() { shutdown(); }

void Loss::initialise()
{
    mGameState.nextSceneID = NO_SCENE;
}

void Loss::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
        mGameState.nextSceneID = MENU;
}

void Loss::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    const char *title = "GAME OVER";
    const char *message = "Press ENTER to return to the menu";

    int titleWidth = MeasureText(title, 60);
    int messageWidth = MeasureText(message, 25);

    DrawText(
        title,
        static_cast<int>(mOrigin.x - titleWidth / 2.0f),
        static_cast<int>(mOrigin.y - 60.0f),
        60,
        RED
    );

    DrawText(
        message,
        static_cast<int>(mOrigin.x - messageWidth / 2.0f),
        static_cast<int>(mOrigin.y + 30.0f),
        25,
        WHITE
    );
}

void Loss::shutdown()
{}