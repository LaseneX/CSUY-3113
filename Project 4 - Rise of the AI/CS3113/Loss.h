#ifndef LOSS_H
#define LOSS_H

#include "Scene.h"

class Loss : public Scene
{
public:
    Loss();
    Loss(Vector2 origin, const char *bgHexCode);
    ~Loss();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // LOSS_H