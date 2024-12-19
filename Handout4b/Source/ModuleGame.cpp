#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Car.h"
#include "Map.h" 

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

    // Crear coche
    car1 = new Car(App->physics, 400, 100, this);

    nitro = new Nitro(App->physics->CreateRectangleSensor(200, 300, 60,40), LoadTexture("Assets/Car.png"),this);

	oil = new OilSlick(App->physics->CreateCircleSensor(400, 500, 20), LoadTexture("Assets/Car.png"), this);


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
    if (IsKeyPressed(KEY_R))
    {
        ray_on = !ray_on;
        ray.x = GetMouseX();
        ray.y = GetMouseY();
    }

    
    App->map->Update();

    
    nitro->Draw();
    oil->Draw();

	
    car1->Update();

    App->particleSystem->Update();


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

    
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        car1->Accelerate();
    }

	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        car1->Brake();
    }

	if (IsKeyDown(KEY_SPACE))
	{
		car1->Nitro();
	}

	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        car1->Turn(-1, true);
    }
	else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        car1->Turn(1, true);
    }
    else
    {
        car1->Turn(0, false); 
    }

    if (car1->isSpinning) {
        float progress = car1->spinningTimeLeft / car1->spinningDuration;

        DrawRectangle(10, 10, 200 * progress, 20, RED); 
        DrawText("Derrapando!", 10, 40, 20, WHITE);
    }

    return UPDATE_CONTINUE;
}


void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
    if (bodyA != nullptr && bodyB != nullptr)
    {
        if (bodyB->colliderType == ColliderType::NITRO)
        {
            Nitro* nitro = static_cast<Nitro*>(bodyB->entity);
            if (nitro && !nitro->isCollected()) 
            {
                b2Vec2 carPosition = car1->body->body->GetPosition();
                Vector2 position = {
                    carPosition.x * PIXELS_PER_METER,
                    carPosition.y * PIXELS_PER_METER
                };
                App->particleSystem->SpawnParticles(position, ParticleType::NITRO);
                nitro->OnPlayerCollision();
                car1->ApplyBoost(15.0f); 
            }
        }
        if (bodyB->colliderType == ColliderType::OIL) {
            if (oil && car1 && !car1->oilCooldownActive) {
                oil->OnPlayerCollision();

                b2Vec2 carPosition = car1->body->body->GetPosition();
                Vector2 position = {
                    carPosition.x * PIXELS_PER_METER,
                    carPosition.y * PIXELS_PER_METER
                };

                App->particleSystem->SpawnParticles(position, ParticleType::SMOKE);

                car1->isSpinning = true;
                car1->spinningTimeLeft = car1->spinningDuration;

                b2Vec2 velocity = car1->body->body->GetLinearVelocity();
                if (velocity.Length() > 0.0f) {
                    car1->preSpinDirection = velocity;
                    car1->preSpinDirection.Normalize();
                }
                else {
                    car1->preSpinDirection.SetZero();
                }

                car1->body->body->SetLinearVelocity(b2Vec2(0, 0));
                car1->oilCooldownActive = true;
                car1->oilCooldownTimeLeft = car1->oilCooldownDuration;
            }
        }

    }
}
