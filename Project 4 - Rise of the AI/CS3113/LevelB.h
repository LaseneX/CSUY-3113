#ifndef LEVELB_H
#define LEVELB_H

#include "LevelA.h"

constexpr int LEVEL_B_WIDTH = 44;

class LevelB : public Scene
{
private:
    unsigned int mLevelData[LEVEL_B_WIDTH * LEVEL_HEIGHT] = {
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 52, 52, 0, 0, 0, 0, 0, 0, 0, 0, 52, 52, 0, 0, 0, 0, 0, 0, 0, 0, 52, 52, 0, 0, 0, 0, 0, 0, 0, 0, 52, 52, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 52, 52, 52, 0, 0, 0, 0, 0, 0, 0, 52, 52, 52, 0, 0, 0, 0, 0, 0, 0, 52, 52, 52, 0, 0, 0, 0, 0, 0, 0, 52, 52, 52, 1,
        1, 13, 13, 13, 0, 0, 0, 13, 13, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 53, 53, 53, 1, 1, 1, 53, 53, 53, 1, 1, 1, 1, 1, 53, 53, 53, 1, 1, 1, 1, 1, 53, 53, 53, 1, 1, 1, 1, 1, 53, 53, 53, 1, 1, 1, 1, 1, 53, 53, 53, 0, 1,
        1, 53, 53, 53, 1, 1, 1, 53, 53, 53, 1, 1, 1, 1, 1, 53, 53, 53, 1, 1, 1, 1, 1, 53, 53, 53, 1, 1, 1, 1, 1, 53, 53, 53, 1, 1, 1, 1, 1, 53, 53, 53, 0, 1
    };
    Entity *mFlyers[5] = {};

public:
    static constexpr float TILE_DIMENSION          = 75.0f,
                           ACCELERATION_OF_GRAVITY = 981.0f,
                           END_GAME_THRESHOLD      = 800.0f;

    LevelB();
    LevelB(Vector2 origin, const char *bgHexCode);
    ~LevelB();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LEVELB_H
