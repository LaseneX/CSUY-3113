#include "LevelA.h"

LevelA::LevelA()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

LevelA::LevelA(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

LevelA::~LevelA() { shutdown(); }

void LevelA::initialise()
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
        LEVEL_WIDTH, LEVEL_HEIGHT, // map grid cols & rows
        mLevelData,                // grid data
        "assets/tilesheet.png",    // texture filepath
        TILE_DIMENSION,            // tile size
        12, 6,                     // texture cols & rows
        mOrigin                    // in-game origin
    );

    /*
        ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> playerAnimationAtlas = {
        { LEFT,  {  9, 10 } }, // Same as right but flipped
        { RIGHT, { 9, 10 } },
    };

    mGameState.player = new Entity(
        { mOrigin.x - 925.0f, mOrigin.y - 200.0f }, // position
        { 54.55f, 75.0f },                          // scale
        "assets/player_tilesheet.png",              // texture file address
        ATLAS,                                      // single image or atlas?
        { 3, 9 },                                   // atlas dimensions
        playerAnimationAtlas,                       // actual atlas
        PLAYER                                      // entity type
    );

    mGameState.player->setJumpingPower(550.0f);
    mGameState.player->setColliderDimensions({
        mGameState.player->getScale().x * 0.95f,
        mGameState.player->getScale().y * 0.9f
    });
    mGameState.player->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });

    mGameState.enemy = new Entity(
        { mOrigin.x + 150.0f, mOrigin.y + 187.5f },
        { 80.0f, 36.0f },
        "assets/wanderingEnemy.png",
        NPC
    );

    mGameState.enemy->setAcceleration({ 0.0f, ACCELERATION_OF_GRAVITY });
    mGameState.enemy->setSpeed(100);
    mGameState.enemy->setAIType(WANDERER);
}

void LevelA::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    mGameState.player->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    if (mGameState.player->getPosition().y > END_GAME_THRESHOLD) { mGameState.nextSceneID = LEVEL_B; }

    mGameState.enemy->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    if (mGameState.nextSceneID == NO_SCENE && mGameState.player->collidesWith(mGameState.enemy))
    {
        gLives--;
        if (gLives <= 0) { mGameState.nextSceneID = LOSE; }
        else { mGameState.nextSceneID = LEVEL_A; }
        return;
    }
}

void LevelA::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.player->render();
    mGameState.map->render();
    mGameState.enemy->render();
}

void LevelA::shutdown()
{
    delete mGameState.player;
    mGameState.player = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;
    delete mGameState.enemy;
    mGameState.enemy = nullptr;


    UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};
    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
    UnloadSound(mGameState.spawnSound);
    mGameState.spawnSound = {};
}
