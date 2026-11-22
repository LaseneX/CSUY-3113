#include "LevelC.h"

extern int gLives;

LevelC::LevelC()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

LevelC::LevelC(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

LevelC::~LevelC() { shutdown(); }

void LevelC::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    mGameState.bgm = LoadMusicStream("assets/Wepa.mp3");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/jump.wav");
    mGameState.spawnSound = LoadSound("assets/spawn.wav");
    PlaySound(mGameState.spawnSound);

    /*
        ----------- MAP -----------
    */
    mGameState.map = new Map(
        LEVEL_C_WIDTH, LEVEL_HEIGHT,
        mLevelData,
        "assets/tilesheet.png",
        TILE_DIMENSION,
        12, 6,
        mOrigin
    );

    /*
        ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> playerAnimationAtlas = {
        { LEFT,  { 9, 10 } },
        { RIGHT, { 9, 10 } },
    };

    mGameState.player = new Entity(
        { mOrigin.x - 1500.0f, mOrigin.y - 50.0f },
        { 54.55f, 75.0f },
        "assets/player_tilesheet.png",
        ATLAS,
        { 3, 9 },
        playerAnimationAtlas,
        PLAYER
    );

    mGameState.player->setJumpingPower(550.0f);
    mGameState.player->setColliderDimensions({
        mGameState.player->getScale().x * 0.95f,
        mGameState.player->getScale().y * 0.9f
    });
    mGameState.player->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

    const float ghostPositions[3] = {
        mOrigin.x - 500.0f,
        mOrigin.x + 400.0f,
        mOrigin.x + 1300.0f
    };

    for (int i = 0; i < 3; i++)
    {
        mGameState.ghosts[i] = new Entity(
            { ghostPositions[i], mOrigin.y - 50.0f },
            { 135.0f / 3.0f , 130.0f / 3.0f },
            "assets/ghost.png",
            NPC
        );

        mGameState.ghosts[i]->setSpeed(100 + i * 25);
        mGameState.ghosts[i]->setAIType(FOLLOWER);
    }
}

void LevelC::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    mGameState.player->update(
        deltaTime,
        nullptr,
        mGameState.map,
        nullptr,
        0
    );

    if (mGameState.player->getPosition().y > END_GAME_THRESHOLD)
    {
        mGameState.nextSceneID = WIN;
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        mGameState.ghosts[i]->update(
            deltaTime,
            mGameState.player,
            mGameState.map,
            nullptr,
            0
        );

        if (mGameState.player->collidesWith(mGameState.ghosts[i]))
        {
            gLives--;

            if (gLives <= 0) { mGameState.nextSceneID = LOSE; }
            else { mGameState.nextSceneID = LEVEL_C; }

            return;
        }
    }
}

void LevelC::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.player->render();
    mGameState.map->render();

    for (int i = 0; i < 3; i++) { mGameState.ghosts[i]->render(); }
}

void LevelC::shutdown()
{
    delete mGameState.player;
    mGameState.player = nullptr;

    delete mGameState.map;
    mGameState.map = nullptr;

    for (int i = 0; i < 3; i++)
    {
        delete mGameState.ghosts[i];
        mGameState.ghosts[i] = nullptr;
    }

    UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};
    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
    UnloadSound(mGameState.spawnSound);
    mGameState.spawnSound = {};
}