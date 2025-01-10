#include "Map.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include <cmath>
#include <iostream>

Map::Map(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

Map::~Map()
{
}

bool Map::Start()
{
    mapTexture = LoadTexture("Assets/map1_fondo.png");

    const int borderPoints[] = {
    340, 80,
    286, 103,
    247, 126,
    200, 170,
    174, 232,
    176, 512,
    194, 556,
    231, 599,
    285, 630,
    358, 646,
    1047, 649,
    1126, 618,
    1167, 583,
    1184, 522,
    1167, 460,
    1118, 413,
    1036, 391,
    575, 390,
    565, 378,
    571, 364,
    1053, 248,
    1103, 217,
    1123, 170,
    1112, 127,
    1090, 101,
    1056, 81,
    1008, 70,
    399, 72
    };


    const int insidePoints[] = {
    408, 159,
    1007, 160,
    1009, 169,
    999, 173,
    526, 288,
    497, 305,
    480, 320,
    464, 343,
    460, 371,
    466, 416,
    500, 451,
    561, 474,
    1039, 476,
    1068, 492,
    1077, 523,
    1068, 542,
    1055, 557,
    1035, 562,
    364, 563,
    332, 552,
    291, 521,
    280, 492,
    278, 344,
    280, 257,
    286, 221,
    305, 197,
    344, 171,
    381, 161
    };

    mapPoints.clear();
    insidePointsVector.clear();

    for (size_t i = 0; i < sizeof(borderPoints) / sizeof(borderPoints[0]); i += 2)
    {
        mapPoints.emplace_back(borderPoints[i], borderPoints[i + 1]);
    }

    for (size_t i = 0; i < sizeof(insidePoints) / sizeof(insidePoints[0]); i += 2)
    {
        insidePointsVector.emplace_back(insidePoints[i], insidePoints[i + 1]);
    }

    CreateBorders(mapPoints, ColliderType::WALL);
    CreateBorders(insidePointsVector, ColliderType::INSIDE);

    return true;
}



update_status Map::Update()
{
    
    DrawTexture(mapTexture, 0, 0, WHITE);
    return UPDATE_CONTINUE;
}

bool ArePointsTooClose(const std::pair<int, int>& p1, const std::pair<int, int>& p2, float threshold)
{
    float dx = (float)(p2.first - p1.first);
    float dy = (float)(p2.second - p1.second);
    bool tooClose = (dx * dx + dy * dy) < (threshold * threshold);

    return tooClose;
}


void Map::CreateBorders(const std::vector<std::pair<int, int>>& points, ColliderType colliderType)
{
    std::vector<int> chainPoints;

    for (const auto& point : points)
    {
        chainPoints.push_back(point.first);
        chainPoints.push_back(point.second);
    }

    if (chainPoints.size() >= 4)
    {
        PhysBody* border = App->physics->CreateChain(0, 0, chainPoints.data(), chainPoints.size(), colliderType);

        if (border != nullptr)
        {
            if (border->body == nullptr || border->body->GetUserData().pointer == 0)
            {
                LOG("Error: PhysBody userData is not set properly.");
            }

            border->listener = this;

            if (colliderType == ColliderType::WALL)
            {
                mapBorders.push_back(border);
            }
            else if (colliderType == ColliderType::INSIDE)
            {
                insideBorders.push_back(border);
            }
        }
    }
    else
    {
        std::cout << "Error: Not enough valid points to create a border.\n";
    }
}




bool Map::CleanUp()
{
    UnloadTexture(mapTexture);

    for (PhysBody* border : mapBorders)
    {
        if (border != nullptr)
        {
            delete border;
            border = nullptr;
        }
    }
    mapBorders.clear();

    return true;
}

void Map::SetMapTexture(Texture2D texture)
{
    mapTexture = texture;
}




