#pragma once

#include "Globals.h"
#include "Module.h"
#include "Car.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:

	std::vector<PhysicEntity*> entities;
	
	Car* car1;
	

	vec2<int> ray;
	bool ray_on;
};
