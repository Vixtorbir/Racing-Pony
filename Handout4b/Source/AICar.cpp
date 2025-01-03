#include "AICar.h"

AICar::AICar(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
    : Car(physics, _x, _y, _listener, _texture), currentWaypointIndex(0) {

	texture = _texture;
}

AICar::~AICar() {}

void AICar::SetWaypoints(const std::vector<Waypoint>& newWaypoints) {
    waypoints = newWaypoints;
    currentWaypointIndex = 0;
}

void AICar::Update(const std::vector<Waypoint>& waypoints) {

    if (waypoints.empty()) return;

    Waypoint target = waypoints[currentWaypointIndex];

    int carX, carY;
    body->GetPhysicPosition(carX, carY);

    float dx = target.x - carX;
    float dy = target.y - carY;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < 30.0f) {
        currentWaypointIndex = (currentWaypointIndex + 1) % waypoints.size();
        return;
    }   

    Accelerate();
}

void AICar::Draw() {
    int carX, carY;
    body->GetPhysicPosition(carX, carY);
    carX -= texture.width / 2;
    carY -= texture.height / 2;

    float rotation = body->GetRotation() * RAD2DEG;

    DrawTexturePro(
        texture,
        Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
        Rectangle{ (float)carX + texture.width / 2, (float)carY + texture.height / 2,
                  (float)texture.width, (float)texture.height },
        Vector2{ (float)texture.width / 2, (float)texture.height / 2 },
        rotation,
        BLACK 
    );
}
