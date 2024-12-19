#include "Boost.h"
#include "Application.h"
#include <iostream>

Nitro::Nitro(PhysBody* body, Texture2D texture, Module* listener)
    : MapItem(body, texture, listener) {
    body->colliderType = ColliderType::NITRO;
    body->entity = this;
}

Nitro::~Nitro() {}

void Nitro::OnPlayerCollision() {
    if (currentCooldown <= 0.0f) {
        std::cout << "Nitro activated!" << std::endl;
        currentCooldown = cooldownTime; 
    }
}

bool Nitro::isAvailable() const {
    return currentCooldown <= 0.0f;
}


void Nitro::Update() {
    if (currentCooldown > 0.0f) {
        currentCooldown -= 1.0f / 60.0f; 
        if (currentCooldown < 0.0f) {
            currentCooldown = 0.0f; 
        }
    }
}
