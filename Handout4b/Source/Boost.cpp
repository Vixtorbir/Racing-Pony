#include "Boost.h"
#include "Application.h"
#include <iostream>

Nitro::Nitro(PhysBody* body, Texture2D texture, Module* listener)
    : MapItem(body, texture, listener)
{
    body->colliderType = ColliderType::NITRO;
    body->entity = this;
}

Nitro::~Nitro()
{
}

void Nitro::OnPlayerCollision()
{
    std::cout << "Nitro activated!" << std::endl;
    collected = true;
}

bool Nitro::isCollected() const
{
    return collected;
}

