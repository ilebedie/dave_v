
#ifndef INDEXES_H
#define INDEXES_H

#include "ecs_types.h"
// Custom hash function for PixelPosition
struct PixelPositionHash {
    size_t operator()(const PixelPosition& k) const {
        return ((hash<short>()(k.px) ^ (hash<short>()(k.py) << 1)) >> 1);
    }
};

// Indexes
using TilePositionIndex = unordered_map<PixelPosition, Entity, PixelPositionHash>;

#endif