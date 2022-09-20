#include "renderer.h"

SDL_Texture* RendererSystem::_getTexture(int tile_index) {
    // Move to texture atlas to a component
    return gameAssets.graphics_tiles[tile_index];
}

void RendererSystem::render() {
    SDL_SetRenderDrawColor(gameWindow.renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(gameWindow.renderer);

    // TODO: combine these components into archetype
    auto tilesComponent = world.components["Tile"];
    auto tiles = (TileComponent *)tilesComponent;
    auto visibilityComponent = world.components["Visibility"];
    auto visibilityP = (VisibilityComponent *)visibilityComponent;
    auto pixelPositionComponent = world.components["PixelPosition"];
    auto pixelPositionP = (PixelPositionComponent *)pixelPositionComponent;

    for (auto it = tiles->begin(); it != tiles->end(); ++it) {
        auto eid = it->first;
        auto tile = it->second;
        auto visibility = (*visibilityP)[eid];
        auto pixelPosition = (*pixelPositionP)[eid];
        if (visibility.visible) {
            SDL_Rect dstrect = {pixelPosition.px, pixelPosition.py, TILE_SIZE, TILE_SIZE};
            SDL_RenderCopy(gameWindow.renderer, _getTexture(tile.tile_index), NULL, &dstrect);
        }
    }

    SDL_RenderPresent(gameWindow.renderer);
}