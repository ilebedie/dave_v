
#include "gravity.h"

void GravitySystem::update() {
    NonFloatingTilesComponent* nonFloatingTilesP = &world.nonFloatingTilesComponent;
    for (auto eid = nonFloatingTilesP->begin(); eid != nonFloatingTilesP->end(); ++eid) {
        auto& pixelPosition = world.pixelPositionComponent[*eid];

        // Some advanced range finding data structure may be used for indexing
        // arbitrary position of floating tiles. But to make it simple, we just
        // assume that tiles are at positions that are multiples of 16.
        // That's first tradeoff. Easy to implement, but not very flexible.
        // TODO: check BSP tree and K D tree for arbitrary position indexing
        if (pixelPosition.py % TILE_SIZE != 0) {
            pixelPosition.py += 1;
            continue;
        }
        short x = pixelPosition.px - pixelPosition.px % TILE_SIZE;
        short y = pixelPosition.py + TILE_SIZE;
        PixelPosition leftTileBelow = {x,y};
        x = x + 16;
        PixelPosition rightTileBelow = {x,y};
        auto noTilesBelow = world.floatingTilesPositionIndex.find(leftTileBelow) == world.floatingTilesPositionIndex.end()
           && world.floatingTilesPositionIndex.find(rightTileBelow) == world.floatingTilesPositionIndex.end();

        if (noTilesBelow) {
            pixelPosition.py += 1;
        }
    }
}