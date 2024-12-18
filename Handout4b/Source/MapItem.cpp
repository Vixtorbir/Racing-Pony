#include "MapItem.h"

MapItem::MapItem(PhysBody* body, Texture2D texture, Module* listener)
    : PhysicEntity(body, listener), texture(texture)
{
    body->listener = listener;
}

MapItem::~MapItem()
{
    UnloadTexture(texture);
}

void MapItem::Update()
{

}

void MapItem::Draw()
{
    int x, y;
    body->GetPhysicPosition(x, y);

    DrawTexture(texture, x - texture.width / 2, y - texture.height / 2, WHITE);
}
