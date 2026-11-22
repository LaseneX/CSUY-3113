#ifndef LEVELA_H
#define LEVELA_H

#include "Scene.h"


constexpr int LEVEL_WIDTH  = 28,
              LEVEL_HEIGHT = 8;

// Entity *mEnemy; // Pointer to wandering enemy

class LevelA : public Scene
{
private:
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = { 
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 52, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 52, 52, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 52, 52, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 53, 53, 53, 0, 1,
        1, 53, 53, 53, 0, 0, 0, 53, 53, 53, 0, 0, 0, 0, 0, 0, 0, 53, 53, 53, 53, 53, 0, 0, 0, 0, 0, 1,
        1, 53, 53, 53, 1, 1, 1, 52, 53, 53, 53, 53, 53, 0, 0, 0, 0, 0, 0, 52, 53, 53, 53, 53, 53, 0, 0, 1,
        1, 53, 53, 53, 1, 52, 13, 53, 53, 53, 53, 53, 53, 53, 52, 1, 1, 52, 52, 53, 53, 53, 53, 53, 53, 53, 0, 1
    };

public:
    static constexpr float TILE_DIMENSION          = 75.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD      = 800.0f;

    LevelA();
    LevelA(Vector2 origin, const char *bgHexCode);
    ~LevelA();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVELA_H
