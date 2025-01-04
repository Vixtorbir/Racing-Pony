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
    mapTexture = LoadTexture("Assets/Map2.png");

    const int borderPoints[] = {
    287, 111,
    240, 144,
    214, 174,
    185, 232,
    183, 509,
    200, 552,
    239, 594,
    291, 625,
    360, 642,
    1046, 639,
    1122, 608,
    1154, 573,
    1170, 515,
    1153, 463,
    1122, 428,
    1035, 395,
    573, 394,
    559, 382,
    566, 364,
    1051, 246,
    1094, 210,
    1109, 172,
    1098, 131,
    1075, 105,
    1051, 90,
    1009, 80,
    379, 78
    };


    const int insidePoints[] = {
        400, 163,
        1000, 159,
        1008, 168,
        512, 295,
        473, 333,
        459, 369,
        468, 417,
        506, 457,
        563, 474,
        1027, 477,
        1062, 496,
        1065, 537,
        1025, 560,
        369, 561,
        301, 527,
        280, 488,
        282, 252,
        305, 208,
        354, 172
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

    if (tooClose)
    {
        std::cout << "Points too close: (" << p1.first << ", " << p1.second
            << ") and (" << p2.first << ", " << p2.second << ")\n";
    }

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

    std::cout << "Creating border with " << chainPoints.size() / 2 << " points.\n";

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

void Map::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA != nullptr && bodyB != nullptr)
	{
		if (bodyA->colliderType == ColliderType::CAR && bodyB->colliderType == ColliderType::WALL)
		{
			std::cout << "Collision with wall detected.\n";
		}
		else if (bodyA->colliderType == ColliderType::WALL && bodyB->colliderType == ColliderType::CAR)
		{
			std::cout << "Collision with wall detected.\n";
		}
	}
}

void Map::SetMapTexture(Texture2D texture)
{
    mapTexture = texture;
}




