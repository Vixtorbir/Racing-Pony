#pragma once

#include "Module.h"
#include "PhysicEntity.h"
#include "ModulePhysics.h"
#include "raylib.h"
#include <string>

class MapItem : public PhysicEntity
{
public:
    MapItem(PhysBody* body, Texture2D texture, Module* listener);
    virtual ~MapItem();

    virtual void OnPlayerCollision() = 0; 

    void Update();
    void Draw();

protected:
    Texture2D texture;
};
