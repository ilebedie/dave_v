
#include "ecs_types.h"
#include "../window/game_window.h"
#include <iostream>

using namespace std;

GameAssets::GameAssets(GameWindow &gameWindow) {
    for(int i = 0; i < 158; i++) {
        auto fileName = "assets/tiles/tile" + std::to_string(i) + ".bmp";
        auto surface = SDL_LoadBMP(fileName.c_str());
        if (surface == nullptr) {
            cout << "Error loading tile " << i << endl;
            terminate();
        }
        graphics_tiles[i] = SDL_CreateTextureFromSurface(gameWindow.renderer, surface);
        SDL_FreeSurface(surface);
    }
}
