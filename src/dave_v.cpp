#include "dave_v.h"

int main(int argc, char* argv[])
{
    GameWindow gameWindow;
    GameAssets assets(gameWindow);
    // TODO: should these state variables also be in ECS?
    GameState game;
    Entities world;
    RendererSystem renderer(world, gameWindow, assets);
    GravitySystem gravitySystem(world);

    while(!game.quit)
    {
        auto timer_begin = SDL_GetTicks();
        gameWindow.checkInput(game);
        renderer.render();
        gravitySystem.update();
        auto timer_end = SDL_GetTicks();
        auto delay = 33 - (timer_end - timer_begin);
        // delay > 33 means that negative overflow happened
        delay = delay > 33 ? 0 : delay;
        SDL_Delay(delay);
    }
    return 0;

}
