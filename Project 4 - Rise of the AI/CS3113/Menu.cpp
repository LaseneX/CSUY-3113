#include "Menu.h"

Menu::Menu() : Scene()
{}

Menu::Menu(Vector2 origin, const char *bgHexCode)
    : Scene(origin, bgHexCode)
{}

void Menu::initialise()
{
    mGameState.player = nullptr;
    mGameState.map = nullptr;
    mGameState.nextSceneID = NO_SCENE;
}

void Menu::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        gLives = INITIAL_LIVES;
        mGameState.nextSceneID = LEVEL_A;
    }
}

void Menu::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    const char *title = "PLATFORMER";
    const char *instruction = "Press ENTER to Start";

    int titleSize = 48;
    int instructionSize = 24;

    DrawText(
        title,
        static_cast<int>(mOrigin.x - MeasureText(title, titleSize) / 2),
        static_cast<int>(mOrigin.y - 60),
        titleSize,
        WHITE
    );

    DrawText(
        instruction,
        static_cast<int>(mOrigin.x - MeasureText(instruction, instructionSize) / 2),
        static_cast<int>(mOrigin.y + 20),
        instructionSize,
        WHITE
    );
}

void Menu::shutdown()
{
    // Nothing to unload.
}