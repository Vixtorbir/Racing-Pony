#pragma once

#include "Globals.h"
#include "Module.h"
#include "p2Point.h"
#include "raylib.h"
#include <vector>
#include "PhysicEntity.h"

class PhysBody;

class Car : public PhysicEntity {
public:
    Car(ModulePhysics* physics, int _x, int _y, Module* _listener);
    ~Car();

   void Update() override;
    bool CleanUp();

    void Accelerate();
    void Brake();
    void Turn(float direction, bool turn);
	void Nitro();

  

public:
    Texture2D texture;

	float currentAcceleration;

    bool nitroActive = false;       
    float nitroTimeLeft = 0.0f;    

    const float nitroFactor = 7.0f; 
    const float nitroDuration = 1.0f;

    const float nitroCooldown = 5.0f; 
    float nitroCooldownTimeLeft = 0.0f;

	
};
