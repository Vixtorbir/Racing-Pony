#include "OilSlick.h"
#include "Application.h"

OilSlick::OilSlick(PhysBody* body, Texture2D texture, Module* listener)
    : MapItem(body, texture, listener)
{
    body->colliderType = ColliderType::OIL;
    body->entity = this;
}

OilSlick::~OilSlick()
{
}

void OilSlick::OnPlayerCollision()
{
}
