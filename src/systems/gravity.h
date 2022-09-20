#ifndef GRAVITY_H
#define GRAVITY_H

#include "../entities/entity.h"

struct GravitySystem {
    Entities &world;
    // Apply gravity to non-floating entity e
    GravitySystem(Entities &world) 
        : world(world) {}

    void update();
};

#endif