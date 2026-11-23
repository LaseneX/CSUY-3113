/**

* Author: Enes Lazim

* Assignment: Rise of the AI

* Date due: [11/22/2026]

* I pledge that I have completed this assignment without

* collaborating with anyone else, in conformance with the

* NYU School of Engineering Policies and Procedures on

* Academic Misconduct.

**/

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
