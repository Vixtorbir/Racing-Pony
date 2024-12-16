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
    mapTexture = LoadTexture("Assets/Map.png");

    mapPoints.clear(); 
    mapPoints.push_back(std::make_pair(0, 0));
    mapPoints.push_back(std::make_pair(0, 720));
    mapPoints.push_back(std::make_pair(1280, 720));
    mapPoints.push_back(std::make_pair(1280, 0));
    mapPoints.push_back(std::make_pair(0, 0));

    CreateBorders();

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


void Map::CreateBorders()
{
    std::vector<int> points;

    for (size_t i = 0; i < mapPoints.size() - 1; ++i) 
    {
        points.push_back(mapPoints[i].first);
        points.push_back(mapPoints[i].second);

        std::cout << "Point " << i << ": ("
            << mapPoints[i].first << ", " << mapPoints[i].second << ")\n";
    }

    std::cout << "Creating border with " << points.size() / 2 << " points.\n";

    
    if (points.size() >= 4) 
    {
        PhysBody* border = App->physics->CreateChain(0, 0, points.data(), points.size());

        if (border != nullptr)
        {
            mapBorders.push_back(border);
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




