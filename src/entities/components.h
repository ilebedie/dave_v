#ifndef COMPONENT_H
#define COMPONENT_H

#include "ecs_types.h"

using namespace std;

template<class ComponentType> using Component = ArrayHashMap<Entity, ComponentType>;
using BoolComponent = unordered_set<Entity>;

// Component definitions
using PixelPositionComponent = Component<PixelPosition>;
using VisibilityComponent = BoolComponent;
using TileComponent = Component<Tile>;
using NonFloatingTilesComponent = BoolComponent;


#endif