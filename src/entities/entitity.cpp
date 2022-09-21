#include "entity.h"

long long generateEid() {
    return 0;
}


Entities::Entities() {
    Entity eid = 0;// generateEid();
    Tile tile = {18}; // visible tile, hangs in the air
    tileComponent[eid] = tile;
    visibilityComponent.insert(eid);
    PixelPosition pixelPosition = {20, 20};
    pixelPositionComponent[eid] = pixelPosition;
    nonFloatingTilesComponent.insert(eid);
    tilesPositionIndex[pixelPosition] = eid;

    Entity eid1 = 1;// generateEid();
    Tile tile1 = {17};
    tileComponent[eid1] = tile1;
    // visibilityComponent.insert(eid1);
    PixelPosition pixelPosition1 = {40, 40};
    pixelPositionComponent[eid1] = pixelPosition1;
    tilesPositionIndex[pixelPosition1] = eid1;

    Entity eid2 = 2;// generateEid();
    Tile tile2 = {20}; // visible tile, falls down
    tileComponent[eid2] = tile2;
    visibilityComponent.insert(eid2);
    PixelPosition pixelPosition2 = {60, 60};
    pixelPositionComponent[eid2] = pixelPosition2;
    tilesPositionIndex[pixelPosition2] = eid2;

    Entity eid3 = 3;// generateEid();
    Tile tile3 = {19}; // visible tile, falls down
    tileComponent[eid3] = tile3;
    visibilityComponent.insert(eid3);
    PixelPosition pixelPosition3 = {20, 80};
    pixelPositionComponent[eid3] = pixelPosition3;
    tilesPositionIndex[pixelPosition3] = eid3;
    
    components["Tile"] = &tileComponent; 
    components["Visibility"] = &visibilityComponent;
    components["PixelPosition"] = &pixelPositionComponent;
    components["NonFloatingTiles"] = &nonFloatingTilesComponent;
}