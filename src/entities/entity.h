#ifndef ENTITY_H
#define ENTITY_H

#include "ecs_types.h"
#include "components.h"
#include "indexes.h"
#include "ecs_constants.h"

struct Entities {
    ArrayHashMap<ArchetypeName, ArchetypeStorage> archetypes;
    ArrayHashMap<ComponentName, ComponentGenericType> components;
    Entities();

    // ComponentGenericType is void *
    // So these definitions are here so that Entities instance would manage 
    // the memory of the components
    // TODO: get rid of void *
    PixelPositionComponent pixelPositionComponent;
    TileComponent tileComponent;
    VisibilityComponent visibilityComponent;
    NonFloatingTilesComponent nonFloatingTilesComponent;

    // Indexes
    TilePositionIndex tilesPositionIndex;
};

#endif