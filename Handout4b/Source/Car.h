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
    Car(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture);
    ~Car();

   void Update() override;
    bool CleanUp();

    void Accelerate();
    void Brake();
    void Turn(float direction, bool turn);
	void Nitro();

    void ApplyBoost(float boostFactor);
   
	void Draw();

  

public:

    Texture2D texture;
    

    bool nitroActive = false;       
    float nitroTimeLeft = 0.0f;    

    
    const float nitroDuration = 1.0f;

    const float nitroCooldown = 3.0f; 
    float nitroCooldownTimeLeft = 0.0f;

    bool isSpinning;              
    float spinningTimeLeft;       
    const float spinningDuration = 2.0f; 
    const float spinningAngularVelocity = 5.0f;

    b2Vec2 preSpinDirection;

    bool oilCooldownActive;
    float oilCooldownTimeLeft;
    const float oilCooldownDuration = 5.0f;

	

	
};
