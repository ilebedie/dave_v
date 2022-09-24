#ifndef ENTITY_H
#define ENTITY_H

#include "ecs_types.h"
#include "components.h"
#include "indexes.h"
#include "ecs_constants.h"

struct Entities {
    Entities();
    void init();
    void reset();

    PixelPositionComponent pixelPositionComponent;
    TileComponent tileComponent;
    VisibilityComponent visibilityComponent;
    NonFloatingTilesComponent nonFloatingTilesComponent;

    // Indexes
    TilePositionIndex floatingTilesPositionIndex;
};

#endif