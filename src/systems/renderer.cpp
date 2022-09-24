#include "renderer.h"

SDL_Texture* RendererSystem::_getTexture(int tile_index) {
    // Move to texture atlas to a component
    return gameAssets.graphics_tiles[tile_index];
}

void RendererSystem::update() {
    gameWindow.fillBlack();

    auto tiles = &world.tileComponent;
    auto visibilityP = &world.visibilityComponent;
    auto pixelPositionP = &world.pixelPositionComponent;

    for (auto it = tiles->begin(); it != tiles->end(); ++it) {
        auto eid = it->first;
        auto tile = it->second;
        auto visibility = visibilityP->find(eid);
        if (visibility != visibilityP->end()) {
            auto pixelPosition = (*pixelPositionP)[eid];
            SDL_Rect dstrect = {
                pixelPosition.px + camera_x,
                pixelPosition.py + camera_y, 
                TILE_SIZE, 
                TILE_SIZE
            };
            SDL_RenderCopy(gameWindow.renderer, _getTexture(tile.tile_index), NULL, &dstrect);
        }
    }

    SDL_RenderPresent(gameWindow.renderer);
}