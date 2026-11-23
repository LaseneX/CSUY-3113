/**

* Author: Enes Lazim

* Assignment: Rise of the AI

* Date due: [11/22/2026]

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

#include "LevelB.h"

LevelB::LevelB()
    : Scene{ { 0.0f, 0.0f }, "#000000" }
{}

LevelB::LevelB(Vector2 origin, const char *bgHexCode)
    : Scene{ origin, bgHexCode }
{}

LevelB::~LevelB() { shutdown(); }

void LevelB::initialise()
{
    mGameState.nextSceneID = NO_SCENE;

    mGameState.bgm = LoadMusicStream("assets/Wepa.mp3");
    SetMusicVolume(mGameState.bgm, 0.33f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("assets/jump.wav");
    mGameState.spawnSound = LoadSound("assets/spawn.wav");
    PlaySound(mGameState.spawnSound);

    const float mapWidth  = LEVEL_B_WIDTH * TILE_DIMENSION;
    const float mapHeight = LEVEL_HEIGHT * TILE_DIMENSION;
    const float mapLeft   = mOrigin.x - mapWidth / 2.0f;
    const float mapRight  = mOrigin.x + mapWidth / 2.0f;
    const float mapTop    = mOrigin.y - mapHeight / 2.0f;

    const float flyerMinX    = mapLeft + 24.0f;
    const float flyerMaxX    = mapRight - 24.0f;
    const float flyerGroundY = mapTop + (LEVEL_HEIGHT - 1) * TILE_DIMENSION - 24.0f;
    const float flyerDiveY   = mOrigin.y;

    /*
        ----------- MAP -----------
    */
    mGameState.map = new Map(
        LEVEL_B_WIDTH, LEVEL_HEIGHT,  // map grid cols & rows
        mLevelData,                 // grid data
        "assets/tilesheet.png",       // texture filepath
        TILE_DIMENSION,             // tile size
        12, 6,                      // texture cols & rows
        mOrigin                     // in-game origin
    );

    /*
        ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> playerAnimationAtlas = {
        { LEFT,  {  9, 10 } }, // Same as right but flipped
        { RIGHT, { 9, 10 } },
    };

    mGameState.player = new Entity(
        { mOrigin.x - 1500.0f, mOrigin.y - 50.0f }, // position
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

    const float flyerPositions[5] = {
        mOrigin.x - 1350.0f,
        mOrigin.x - 675.0f,
        mOrigin.x,
        mOrigin.x + 675.0f,
        mOrigin.x + 1350.0f
    };

    for (int i = 0; i < 5; i++)
    {
        mGameState.flyingEnemies[i] = new Entity(
            { flyerPositions[i], mOrigin.y + 100.0f },
            { 48.0f, 48.0f },
            "assets/raven.png",
            NPC
        );

        mGameState.flyingEnemies[i]->setSpeed(100);
        mGameState.flyingEnemies[i]->setAIType(FLYER);

        if (i % 2 != 0) { mGameState.flyingEnemies[i]->moveLeft(); }
        else { mGameState.flyingEnemies[i]->moveRight(); }

        mGameState.flyingEnemies[i]->setFlyerBounds(
            flyerMinX,
            flyerMaxX,
            flyerGroundY,
            flyerDiveY
        );
    }
}

void LevelB::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    mGameState.player->update(
        deltaTime,      // delta time / fixed timestep
        nullptr,        // player
        mGameState.map, // map
        nullptr,        // collidable entities
        0               // col. entity count
    );

    if (mGameState.player->getPosition().y > END_GAME_THRESHOLD) { mGameState.nextSceneID = LEVEL_C; }

    for (int i = 0; i < 5; i++) // As there are 5 ravens for loop is needed
    {
        mGameState.flyingEnemies[i]->update(
            deltaTime,      // delta time / fixed timestep
            nullptr,        // player
            mGameState.map, // map
            nullptr,        // collidable entities
            0               // col. entity count
        );

        if (mGameState.nextSceneID == NO_SCENE && mGameState.player->collidesWith(mGameState.flyingEnemies[i]))
        {
            gLives--;

            if (gLives <= 0) { mGameState.nextSceneID = LOSE; }
            else { mGameState.nextSceneID = LEVEL_B; }

            return;
        }
    }

}

void LevelB::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.player->render();
    mGameState.map->render();
    
    for (int i = 0; i < 5; i++) { mGameState.flyingEnemies[i]->render(); }
}

void LevelB::shutdown()
{
    delete mGameState.player;
    mGameState.player = nullptr;
    delete mGameState.map;
    mGameState.map = nullptr;
    
    for (int i = 0; i < 5; i++)
    {
        delete mGameState.flyingEnemies[i];
        mGameState.flyingEnemies[i] = nullptr;
    }

    UnloadMusicStream(mGameState.bgm);
    mGameState.bgm = {};
    UnloadSound(mGameState.jumpSound);
    mGameState.jumpSound = {};
    UnloadSound(mGameState.spawnSound);
    mGameState.spawnSound = {};
}
