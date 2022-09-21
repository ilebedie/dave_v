#ifndef MOVER_H
#define MOVER_H

#include "../entities/entity.h"

// It moves Dave
struct MoverSystem {
    Entities &world;
    GameInput &input;
    // Apply gravity to non-floating entity e
    MoverSystem(Entities &world, GameInput &input) 
        : world(world), input(input) {}

    void update();
};

#endif