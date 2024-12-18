#pragma once
#include "MapItem.h"

class OilSlick : public MapItem
{
public:
    OilSlick(PhysBody* body, Texture2D texture, Module* listener);
    ~OilSlick();

    void OnPlayerCollision() override;
};
