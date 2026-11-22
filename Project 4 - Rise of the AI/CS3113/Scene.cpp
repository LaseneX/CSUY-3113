#include "Scene.h"

Scene::Scene() : mGameState{
    nullptr, // player
    nullptr, // enemy
    {},      // flying enemies
    {},      // ghost
    nullptr, // map
    {},      // bgm
    {},      // jumpSound
    {},      // spawnSound
    NO_SCENE
    }, 
    mOrigin{} {}

Scene::Scene(Vector2 origin, const char *bgHexCode) : mGameState{
    nullptr, // player
    nullptr, // enemy
    {},      // flying enemies
    {},      // ghost
    nullptr, // map
    {},      // bgm
    {},      // jumpSound
    {},      // spawnSound
    NO_SCENE
    }, 
    mOrigin{ origin }, mBGColourHexCode{ bgHexCode }
{}
