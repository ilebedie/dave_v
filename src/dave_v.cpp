#include "dave_v.h"

GameWindow gameWindow;
GameAssets assets(gameWindow);
Entities world;
RendererSystem renderer(world, gameWindow, assets);
GravitySystem gravitySystem(world);
MoverSystem moverSystem(world, gameWindow.input);

void update() {
    // gravitySystem.update();
    moverSystem.update();
    renderer.update();
}

int main(int argc, char* argv[])
{
    while(!gameWindow.input.quit)
    {
        gameWindow.input.reset();
        auto timer_begin = SDL_GetTicks();
        gameWindow.checkInput();
        update();
        auto timer_end = SDL_GetTicks();
        auto delay = 33 - (timer_end - timer_begin);
        // delay > 33 means that negative overflow happened
        delay = delay > 33 ? 0 : delay;
        SDL_Delay(delay);
    }

    return 0;
}
