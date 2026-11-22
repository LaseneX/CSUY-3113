#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

constexpr int INITIAL_LIVES = 3;
extern int gLives;
struct GameState
{
    Entity *player;
    Entity *enemy;
    Entity *flyingEnemies[5];
    Entity *ghosts[3];

    Map    *map;

    Music bgm;
    Sound jumpSound;
    Sound spawnSound;

    SceneID nextSceneID;
};

class Scene
{
protected:
    GameState   mGameState;
    Vector2     mOrigin;
    const char *mBGColourHexCode = "#000000";

public:
    Scene();
    Scene(Vector2 origin, const char *bgHexCode);
    virtual ~Scene() {}

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() = 0;

    GameState   getState()           const { return mGameState;        }
    Vector2     getOrigin()          const { return mOrigin;           }
    const char *getBGColourHexCode() const { return mBGColourHexCode; }
    SceneID getNextSceneID() const { return mGameState.nextSceneID; }
    void setNextSceneID(SceneID id) { mGameState.nextSceneID = id; }
};

#endif // SCENE_H
