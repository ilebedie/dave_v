#include "mover.h"

void MoverSystem::update() {
    auto tilesP = &world.tileComponent;
    auto nonFloatingTilesP = &world.nonFloatingTilesComponent;
    auto tilesPositionIndexP = &world.floatingTilesPositionIndex;
    Entity eid = 0; // Dave 

    if(!input.try_left && !input.try_right ) {
        return;
    }

    short move_x = input.try_left ? -1 : 1;
    for (auto eid = nonFloatingTilesP->begin(); eid != nonFloatingTilesP->end(); ++eid) {
        PixelPosition &pixelPosition = world.pixelPositionComponent[*eid];
        if (pixelPosition.px % TILE_SIZE != 0) {
            pixelPosition.px += move_x;
            continue;
        }

        if (input.try_right) {
            short x = pixelPosition.px + TILE_SIZE;
            short y = pixelPosition.py - pixelPosition.py % TILE_SIZE;
            PixelPosition rightTileUpper = {x,y};
            y += TILE_SIZE;
            PixelPosition rightTileLower = {x,y};
            bool noTilesRight = \
                tilesPositionIndexP->find(rightTileUpper) == tilesPositionIndexP->end()
                && tilesPositionIndexP->find(rightTileLower) == tilesPositionIndexP->end();
            if (noTilesRight) {
                pixelPosition.px += move_x;
            }
        }
        else {
            short x = pixelPosition.px - TILE_SIZE;
            short y = pixelPosition.py - pixelPosition.py % TILE_SIZE;
            PixelPosition leftTileUpper = {x,y};
            y += TILE_SIZE;
            PixelPosition leftTileLower = {x,y};
            bool noTilesLeft = \
                tilesPositionIndexP->find(leftTileUpper) == tilesPositionIndexP->end()
                && tilesPositionIndexP->find(leftTileLower) == tilesPositionIndexP->end();
            if (noTilesLeft) {
                pixelPosition.px += move_x;
            }
        }
    }
}