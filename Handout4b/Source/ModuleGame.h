#pragma once

#include "Globals.h"
#include "Module.h"
#include "Car.h"
#include "Boost.h"
#include "OilSlick.h"
#include "Checkpoint.h"
#include "UI.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>
#include <cfloat> 

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
	void ResetCheckpoints();
	void UpdateLapTime();
	void CheckBestLap();

public:

	std::vector<Checkpoint*> checkpoints; 
	int currentCheckpointIndex;

	int totalLaps = 3;
	int lapsCompleted = 0;
	float lapStartTime = 0.0f;
	float currentLapTime = 0.0f;
	float bestLapTime = FLT_MAX;

	UI* ui = nullptr;

	std::vector<PhysicEntity*> entities;
	
	Car* car1;
	Nitro* nitro;
	OilSlick* oil;
	

	vec2<int> ray;
	bool ray_on;
};