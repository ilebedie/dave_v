#ifndef RENDERER_H
#define RENDERER_H

#include "../window/game_window.h"
#include "../entities/entity.h"

struct RendererSystem {
    Entities &world;
    GameWindow &gameWindow;
    GameAssets &gameAssets;
    short camera_x = 0;
    short camera_y = 0;

    RendererSystem(Entities &world, GameWindow &gameWindow, GameAssets &gameAssets) 
        : world(world), gameWindow(gameWindow), gameAssets(gameAssets) {}
    void update();
    SDL_Texture* _getTexture(int tile_index);
};

#endif