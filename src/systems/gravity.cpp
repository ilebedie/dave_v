
#include "gravity.h"

void GravitySystem::update() {
    auto tilesComponent = world.components["Tile"];
    auto tilesP = (TileComponent *)tilesComponent;
    auto nonFloatingTilesComponent = world.components["NonFloatingTiles"];
    auto nonFloatingTilesP = (NonFloatingTilesComponent *)nonFloatingTilesComponent;
    auto tilesPositionIndexP = &world.tilesPositionIndex;
    for (auto eid = nonFloatingTilesP->begin(); eid != nonFloatingTilesP->end(); ++eid) {
        auto& pixelPosition = world.pixelPositionComponent[*eid];

        short tileBelowY = pixelPosition.py + TILE_SIZE;
        PixelPosition tileBelow {pixelPosition.px, tileBelowY};

        if (tilesPositionIndexP->find(tileBelow) == tilesPositionIndexP->end()) {
            tilesPositionIndexP->erase(pixelPosition);
            pixelPosition.py += 1;
            world.pixelPositionComponent[*eid] = pixelPosition;
            tilesPositionIndexP->insert({pixelPosition, *eid});
        }
    }
}