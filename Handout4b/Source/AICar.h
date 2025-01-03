#pragma once
#include "Car.h"
#include <vector>

struct Waypoint {
    float x, y;
};

class AICar : public Car {
public:
    AICar(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture);
    ~AICar();

    void Update(const std::vector<Waypoint>& waypoints);
    void Draw();

    void SetWaypoints(const std::vector<Waypoint>& newWaypoints);

private:
    int currentWaypointIndex; 
    std::vector<Waypoint> waypoints;

	Texture2D texture;

};
