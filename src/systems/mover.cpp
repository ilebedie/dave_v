#include "mover.h"

void MoverSystem::update() {
    auto tilesComponent = world.components["Tile"];
    auto tilesP = (TileComponent *)tilesComponent;
    auto nonFloatingTilesComponent = world.components["NonFloatingTiles"];
    auto nonFloatingTilesP = (NonFloatingTilesComponent *)nonFloatingTilesComponent;
    auto tilesPositionIndexP = &world.tilesPositionIndex;
    Entity eid = 0; // Dave 

    if(!input.try_left && !input.try_right ) {
        return;
    }

    auto& pixelPosition = world.pixelPositionComponent[eid];
    short nextTileX = pixelPosition.px + (input.try_left ? -TILE_SIZE : TILE_SIZE);
    PixelPosition tileLeft {nextTileX, pixelPosition.py};
    if (tilesPositionIndexP->find(tileLeft) == tilesPositionIndexP->end()) {
        tilesPositionIndexP->erase(pixelPosition);
        pixelPosition.px += (input.try_left ?  -1: 1);
        world.pixelPositionComponent[eid] = pixelPosition;
        tilesPositionIndexP->insert({pixelPosition, eid});
    }
}