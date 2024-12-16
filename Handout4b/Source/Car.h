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
    void Turn(float direction);

private:
    Texture2D texture;
	float currentAcceleration;
};
