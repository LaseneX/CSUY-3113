#ifndef MENU_H
#define MENU_H

#include "Scene.h"

class Menu : public Scene
{
public:
    Menu();
    Menu(Vector2 origin, const char *bgHexCode);

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // MENU_H