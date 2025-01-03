#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Car.h"
#include "Map.h" 


ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled), selectedCharacter(0)
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

    character1Texture = LoadTexture("Assets/Car.png");
    character2Texture = LoadTexture("Assets/Car.png"); //Crear otra textura para la ia

    selectedCharacter = 0;

    nitro = new Nitro(App->physics->CreateRectangleSensor(200, 300, 20,20), LoadTexture("Assets/nitro.png"),this);

	oil = new OilSlick(App->physics->CreateCircleSensor(400, 580, 15), LoadTexture("Assets/stain.png"), this);

    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(440, 115, 10, 90), 0));
    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(750, 433, 10, 80), 1));
    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(550, 600, 10, 90), 2));
    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(235, 340, 92, 10), 3)); 

    menu = LoadTexture("Assets/menuprincipal.png");

    game_over_menu = LoadTexture("Assets/MapComponents/gameovermenu.png");

    pausemenu = LoadTexture("Assets/pausemenu.png");

    bonus_fx= LoadSound("Assets/music/bonus_sfx.wav");
    car_fx = LoadSound("Assets/music/car_sfx.wav");
    oil_fx = LoadSound("Assets/music/oil_sfx.wav");

    ResetCheckpoints();
    lapsCompleted = 0;

    ui = new UI(totalLaps);

    trafficLight = new TrafficLight();
    trafficLight->StartCountdown(3.0f); 
    canControlCar = false;
    


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


    switch (game_state) {


    case GameState::START_MENU:
        DrawTexture(menu, 0, 0, WHITE);
        if (IsKeyPressed(KEY_ENTER))
        {

            game_state = GameState::SELECT_CHARACTER_MENU;


        }
        break;
    case GameState::SELECT_CHARACTER_MENU:

        DrawTexture(character1Texture, GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 - character1Texture.height / 2, WHITE);
        DrawTexture(character2Texture, GetScreenWidth() / 2 + 100, GetScreenHeight() / 2 - character2Texture.height / 2, WHITE);

        if (selectedCharacter == 0) {
            DrawRectangleLines(GetScreenWidth() / 2 - 210, GetScreenHeight() / 2 - character1Texture.height / 2 - 10,
                character1Texture.width + 20, character1Texture.height + 20, YELLOW);
        }
        else {
            DrawRectangleLines(GetScreenWidth() / 2 + 90, GetScreenHeight() / 2 - character2Texture.height / 2 - 10,
                character2Texture.width + 20, character2Texture.height + 20, YELLOW);
        }

        if (IsKeyPressed(KEY_RIGHT)) selectedCharacter = 1;
        if (IsKeyPressed(KEY_LEFT)) selectedCharacter = 0;

        if (IsKeyPressed(KEY_ENTER)) {
            
            Texture2D playerTexture = (selectedCharacter == 0) ? character1Texture : character2Texture;
            Texture2D aiTexture = (selectedCharacter == 0) ? character2Texture : character1Texture;

           
            car1 = new Car(App->physics, 400, 130, this, playerTexture);
          //aiCar = new AICar(App->physics, 400, 100, this, aiTexture);
          //
          //
          //waypoints = {
          //    {440, 115},
          //    {640, 115},
          //    {840, 115},
          //    {1030, 133},
          //    {1040, 180},
          //    {750, 303},
          //    {550, 600},
          //    {235, 340}
          //};
          //aiCar->SetWaypoints(waypoints);

            game_state = GameState::PLAYING;
        }
        break;

    case GameState::PLAYING:

        if (IsKeyPressed(KEY_Q)) {


            game_state = GameState::PAUSED;

        }

        for (const Waypoint& wp : waypoints) {
            DrawCircle(wp.x, wp.y, 10, BLACK); 
        }


        car1->Draw();
      //  aiCar->Draw();


        trafficLight->Update();

        if (!trafficLight->IsCountdownFinished()) {
            trafficLight->Draw();
            return UPDATE_CONTINUE; 
        }
        else if (!canControlCar) {
            canControlCar = true;
            lapStartTime = GetTime(); 
        }

        if (canControlCar) 
        {
            App->map->Update();
            nitro->Update();
            nitro->Draw();
            oil->Draw();
            car1->Update();
            car1->Draw();

         //   aiCar->Update(waypoints);
         //   aiCar->Draw();

            App->particleSystem->Update();
            UpdateLapTime();
            ui->Update(currentLapTime, bestLapTime, lapsCompleted);
            ui->Draw();

            for (const Waypoint& wp : waypoints) {
                DrawCircle(wp.x, wp.y, 10, BLACK); // Dibuja un círculo negro en cada waypoint esto esta para hacer debug luego se quita
            }


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
        }

        break;

    case GameState::PAUSED:
        DrawTexture(pausemenu, 0, 0, WHITE);
        if (IsKeyPressed(KEY_Q)) {


            game_state = GameState::PLAYING;

        }
        break;

    }


    if (game_state == GameState::GAME_OVER)
    {
        //Draw game over menu
        DrawTexture(game_over_menu, 0, 0, WHITE);

    }
    return UPDATE_CONTINUE;
}


void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
    if (bodyA != nullptr && bodyB != nullptr)
    {
        if (bodyB->colliderType == ColliderType::CHECKPOINT) {
            Checkpoint* checkpoint = static_cast<Checkpoint*>(bodyB->entity);
            if (checkpoint) {
                if (checkpoint->index == 0) {
                    bool allCheckpointsActive = true;
                    for (Checkpoint* cp : checkpoints) {
                        if (!cp->isActive) {
                            allCheckpointsActive = false;
                            break;
                        }
                    }

                    if (allCheckpointsActive) {
                        float currentLapTime = GetTime() - lapStartTime;

                        if (bestLapTime == 0.0f || currentLapTime < bestLapTime) {
                            bestLapTime = currentLapTime;
                            LOG("New best lap time: %.2f seconds", bestLapTime);
                        }
                        lapStartTime = GetTime();

                        lapsCompleted++;
                        ResetCheckpoints();
                        LOG("Lap completed! Total laps: %d", lapsCompleted);
                        LOG("Current lap time: %.2f seconds", currentLapTime);
                    }

                    checkpoint->isActive = true;
                    currentCheckpointIndex = 1;
                }
                else if (checkpoint->index == currentCheckpointIndex) {
                    checkpoint->isActive = true;
                    currentCheckpointIndex++;
                }
            }
        }


        if (bodyB->colliderType == ColliderType::NITRO) {
            Nitro* nitro = static_cast<Nitro*>(bodyB->entity);
            if (nitro && nitro->isAvailable()) { 
                nitro->OnPlayerCollision(); 
                PlaySound(bonus_fx);
                car1->ApplyBoost(15.0f);

                b2Vec2 carPosition = car1->body->body->GetPosition();
                Vector2 position = {
                    carPosition.x * PIXELS_PER_METER,
                    carPosition.y * PIXELS_PER_METER
                };
                App->particleSystem->SpawnParticles(position, ParticleType::NITRO);
            }
        }
        if (bodyB->colliderType == ColliderType::OIL) {
            if (oil && car1 && !car1->oilCooldownActive) {
                oil->OnPlayerCollision();
                PlaySound(oil_fx);

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

void ModuleGame::ResetCheckpoints() {
    
    //añadir sonido que pasas por la meta

    currentCheckpointIndex = 0;
    for (Checkpoint* checkpoint : checkpoints) {
        checkpoint->isActive = false;
    }
}

void ModuleGame::UpdateLapTime() {
    currentLapTime = GetTime() - lapStartTime; 
}




