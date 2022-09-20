#ifndef RENDERER_H
#define RENDERER_H

#include "../window/game_window.h"
#include "../entities/entity.h"

struct RendererSystem {
    Entities &world;
    GameWindow &gameWindow;
    GameAssets &gameAssets;
    // Render entity e at position p

    RendererSystem(Entities &world, GameWindow &gameWindow, GameAssets &gameAssets) 
        : world(world), gameWindow(gameWindow), gameAssets(gameAssets) {}
    void render();
    SDL_Texture* _getTexture(int tile_index);
};

#endif