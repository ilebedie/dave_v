#include "entity.h"
#include "../systems/gravity.h"

long long generateEid() {
    return 0;
}

void Entities::init() {
    Entity eid = 0;// generateEid();
    Tile tile = {18}; 
    tileComponent[eid] = tile;
    visibilityComponent.insert(eid);
    PixelPosition pixelPosition = {20, 20};
    pixelPositionComponent[eid] = pixelPosition;
    nonFloatingTilesComponent.insert(eid);

    Entity eid1 = 1;// generateEid();
    Tile tile1 = {17};
    tileComponent[eid1] = tile1;
    visibilityComponent.insert(eid1);
    PixelPosition pixelPosition1 = {32, 64};
    pixelPositionComponent[eid1] = pixelPosition1;
    floatingTilesPositionIndex[pixelPosition1] = eid1;

    Entity eid2 = 2;// generateEid();
    Tile tile2 = {20}; 
    tileComponent[eid2] = tile2;
    visibilityComponent.insert(eid2);
    PixelPosition pixelPosition2 = {64, 80};
    pixelPositionComponent[eid2] = pixelPosition2;
    floatingTilesPositionIndex[pixelPosition2] = eid2;

    Entity eid3 = 3;// generateEid();
    Tile tile3 = {19}; 
    tileComponent[eid3] = tile3;
    visibilityComponent.insert(eid3);
    PixelPosition pixelPosition3 = {32, 96};
    pixelPositionComponent[eid3] = pixelPosition3;
    floatingTilesPositionIndex[pixelPosition3] = eid3;
}

Entities::Entities() {
    init();
}

void Entities::reset() {
    tileComponent.clear();
    nonFloatingTilesComponent.clear();
    visibilityComponent.clear();
    pixelPositionComponent.clear();
    floatingTilesPositionIndex.clear();
    init();
}