#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Car.h"

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    ray_on = false;
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
    LOG("Loading Game assets");
    bool ret = true;

    App->renderer->camera.x = App->renderer->camera.y = 0;

    car1 = new Car(App->physics, 100, 100, this);

    return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
    LOG("Unloading Game scene");

    if (car1 != nullptr)
    {
        delete car1;
        car1 = nullptr;
    }

    return true;
}


update_status ModuleGame::Update()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        ray_on = !ray_on;
        ray.x = GetMouseX();
        ray.y = GetMouseY();
    }

   
    car1->Update();

    
    int carX, carY;
    car1->body->GetPhysicPosition(carX, carY);

    
    carX -= car1->texture.width / 2;
    carY -= car1->texture.height / 2;

    float rotation = car1->body->GetRotation() * RAD2DEG; 

    DrawTexturePro(car1->texture,
        Rectangle{ 0, 0, (float)car1->texture.width, (float)car1->texture.height }, 
        Rectangle{ (float)carX + car1->texture.width / 2, (float)carY + car1->texture.height / 2,
                   (float)car1->texture.width, (float)car1->texture.height }, 
        Vector2{ (float)car1->texture.width / 2, (float)car1->texture.height / 2 },
        rotation, 
        WHITE);

    
    if (IsKeyDown(KEY_UP))
    {
        car1->Accelerate();
    }

    if (IsKeyDown(KEY_DOWN))
    {
        car1->Brake();
    }

    if (IsKeyDown(KEY_LEFT))
    {
        car1->Turn(-1, true);
    }
    else if (IsKeyDown(KEY_RIGHT))
    {
        car1->Turn(1, true);
    }
    else
    {
        car1->Turn(0, false); 
    }

    return UPDATE_CONTINUE;
}


void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
}
