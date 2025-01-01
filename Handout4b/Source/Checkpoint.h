#pragma once

#include "Globals.h"
#include "Module.h"
#include "p2Point.h"
#include "raylib.h"
#include <vector>
#include "PhysicEntity.h"


class Checkpoint {
public:
    Checkpoint(PhysBody* body, int index) : body(body), index(index), isActive(false) {
        body->colliderType = ColliderType::CHECKPOINT;
        body->entity = this;
    }

    bool isActive;
    int index;
    PhysBody* body;

    
};