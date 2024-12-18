#pragma once
#include "MapItem.h"

class Nitro : public MapItem
{
public:
    Nitro(PhysBody* body, Texture2D texture, Module* listener);
    ~Nitro();

    void OnPlayerCollision() override;
    bool isCollected() const;

private:
	bool collected;
};

