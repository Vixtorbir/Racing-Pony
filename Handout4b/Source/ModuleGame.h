#pragma once

#include "Globals.h"
#include "Module.h"
#include "Car.h"
#include "Boost.h"
#include "OilSlick.h"
#include "Checkpoint.h"
#include "UI.h"
#include "TrafficLight.h"
#include "AICar.h"
#include "MenuManager.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>
#include <cfloat> 

class PhysBody;
class PhysicEntity;

enum class GameState
{
	PLAYING,
	RESTART,
	GAME_OVER,
	PAUSED,
	START_MENU,
	SELECT_CHARACTER_MENU,
	SELECT_MAP_MENU

};

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
	GameState game_state = GameState::START_MENU;

	Sound bonus_fx;
	Sound car_fx;
	Sound oil_fx;

	vec2<int> ray;
	bool ray_on;

	TrafficLight* trafficLight;
	bool canControlCar;

	int selectedCharacter = 0;
	int selectedMap = 0;    
	bool iceMap = false;

	MenuManager* menuManager;

};