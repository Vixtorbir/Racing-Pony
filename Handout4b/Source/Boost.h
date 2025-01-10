#pragma once
#include "MapItem.h"

class Nitro : public MapItem {
public:
    Nitro(PhysBody* body, Texture2D texture, Module* listener);
    ~Nitro();

    void OnPlayerCollision() override;
    bool isAvailable() const;

    void Update();

private:
    float cooldownTime = 0.0f; 
    float currentCooldown = 0.0f;
};
