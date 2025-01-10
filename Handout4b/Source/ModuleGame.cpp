#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Car.h"
#include "Map.h" 
#include <algorithm> 



ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled), selectedCharacter(0), iceMap(false)
{
    ray_on = false;

}

ModuleGame::~ModuleGame()
{}


bool ModuleGame::Start()
{
    LOG("Loading Game assets");
    bool ret = true;

    App->renderer->camera.x = App->renderer->camera.y = 0;

    menuManager = new MenuManager();

    menuManager->LoadAssets();

    selectedCharacter = 0;
    selectedMap = 0;

    nitro = new Nitro(App->physics->CreateRectangleSensor(200, 300, 20, 20), LoadTexture("Assets/nitro.png"), this);

    oil = new OilSlick(App->physics->CreateCircleSensor(400, 580, 15), LoadTexture("Assets/stain.png"), this);

    green_light = LoadTexture("Assets/GREENLIGHT.png");
    red_light = LoadTexture("Assets/REDLIGHT.png");

    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(440, 115, 10, 90), 0));
    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(750, 433, 10, 80), 1));
    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(550, 600, 10, 90), 2));
    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(235, 340, 92, 10), 3));

    checkpointsActiveCar1 = std::vector<bool>(checkpoints.size(), false);
    checkpointsActiveCar2 = std::vector<bool>(checkpoints.size(), false);


    bonus_fx = LoadSound("Assets/music/bonus_sfx.wav");
    car_fx = LoadSound("Assets/music/car_sfx.wav");
    oil_fx = LoadSound("Assets/music/oil_sfx.wav");
    finish_line_fx = LoadSound("Assets/music/bonus_sfx.wav");
    red_light_fx = LoadSound("Assets/music/REDLIGHT.wav");
    victory_fx = LoadSound("Assets/music/victory.wav");

    playingMusic = LoadMusicStream("Assets/music/Playing_Music.wav");
    PlayMusicStream(playingMusic);
    SetMusicVolume(playingMusic, 0.03f);

    ResetCheckpoints();
    lapsCompletedCar1 = 0;
    currentLapTimeCar1 = 0.0f;

    lapsCompletedCar2 = 0;
    currentLapTimeCar2 = 0.0f;

    ui = new UI(totalLaps);

    trafficLight = new TrafficLight();
    trafficLight->Initialize();
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

    if (nitro != nullptr)
    {
        delete nitro;
        nitro = nullptr;
    }

    if (oil != nullptr)
    {
        delete oil;
        oil = nullptr;
    }

    for (Checkpoint* checkpoint : checkpoints) {
        delete checkpoint;
    }
    checkpoints.clear();

    if (ui != nullptr)
    {
        delete ui;
        ui = nullptr;
    }

    if (trafficLight != nullptr)
    {
        delete trafficLight;
        trafficLight = nullptr;
    }

    if (menuManager != nullptr)
    {
        delete menuManager;
        menuManager = nullptr;
    }

    UnloadSound(bonus_fx);
    UnloadSound(car_fx);
    UnloadSound(oil_fx);
    UnloadSound(finish_line_fx);
    UnloadSound(red_light_fx);
    UnloadSound(victory_fx);

    UnloadMusicStream(playingMusic);

    return true;
}



update_status ModuleGame::Update()
{

    if (lapsCompletedCar1 >= totalLaps || lapsCompletedCar2 >= totalLaps) {

        if (car1 != nullptr)
        {
            delete car1;
            car1 = nullptr;
        }
        if (car2 != nullptr)
        {
            delete car2;
            car2 = nullptr;
        }
        game_state = GameState::WIN;
    }
    if (IsKeyPressed(KEY_R))
    {
        ray_on = !ray_on;
        ray.x = GetMouseX();
        ray.y = GetMouseY();
    }

    UpdateMusicStream(playingMusic);

    switch (game_state) {

    case GameState::START_MENU:

        menuManager->DrawMainMenu();

        if (IsKeyPressed(KEY_ENTER))
        {
            game_state = GameState::SELECT_CHARACTER_MENU;
        }
        break;

    case GameState::SELECT_CHARACTER_MENU:

        menuManager->DrawCharacterSelectMenu(selectedCharacter);

        if (IsKeyPressed(KEY_RIGHT)) selectedCharacter = (selectedCharacter + 1) % 5;
        if (IsKeyPressed(KEY_LEFT)) selectedCharacter = (selectedCharacter - 1 + 5) % 5;

        if (IsKeyPressed(KEY_ENTER)) {
            Texture2D carTexture;
            Texture2D carTexture2;

            switch (selectedCharacter) {
            case 0:
                carTexture = menuManager->GetCharacter1Texture();
                break;
            case 1:
                carTexture = menuManager->GetCharacter2Texture();
                break;
            case 2:
                carTexture = menuManager->GetCharacter3Texture();
                break;
            case 3:
                carTexture = menuManager->GetCharacter4Texture();
                break;
            case 4:
                carTexture = menuManager->GetCharacter5Texture();
                break;
            }

            int randomIndex;
            do {
                randomIndex = GetRandomValue(0, 4);  
                switch (randomIndex) {
                case 0: carTexture2 = menuManager->GetCharacter1Texture(); break;
                case 1: carTexture2 = menuManager->GetCharacter2Texture(); break;
                case 2: carTexture2 = menuManager->GetCharacter3Texture(); break;
                case 3: carTexture2 = menuManager->GetCharacter4Texture(); break;
                case 4: carTexture2 = menuManager->GetCharacter5Texture(); break;
                }
            } while (carTexture2.id == carTexture.id);  

            car1 = new Car(App->physics, 400, 130, this, carTexture);
            car2 = new Car(App->physics, 400, 100, this, carTexture2);

            game_state = GameState::SELECT_MAP_MENU;
        }
        break;

    case GameState::SELECT_MAP_MENU:

        menuManager->DrawMapSelectMenu(selectedMap);

        if (IsKeyPressed(KEY_RIGHT)) selectedMap = 1;
        if (IsKeyPressed(KEY_LEFT)) selectedMap = 0;

        if (IsKeyPressed(KEY_ENTER)) {
            App->map->SetMapTexture((selectedMap == 0) ? menuManager->GetMap1Full() : menuManager->GetMap2Full());
            iceMap = (selectedMap == 1);

            if (car1 != nullptr) {
                car1->SetIceMap(iceMap);
            }
            if (car2 != nullptr) {
                car2->SetIceMap(iceMap);
            }

            game_state = GameState::SELECT_GAME_MODE;
        }
        break;

    case GameState::SELECT_GAME_MODE:

        menuManager->DrawGameModeSelectionMenu(selectedMode);

        if (IsKeyPressed(KEY_RIGHT)) selectedMode = 1;
        if (IsKeyPressed(KEY_LEFT)) selectedMode = 0;

        if (IsKeyPressed(KEY_ENTER)) {

            if (selectedMode == 0) {

                App->map->SetMapTexture((selectedMap == 0) ? menuManager->GetMap1Full() : menuManager->GetMap2Full());
                iceMap = (selectedMap == 1);

                if (car1 != nullptr) {
                    car1->SetIceMap(iceMap);
                }
                if (car2 != nullptr) {
                    car2->SetIceMap(iceMap);
                }
                game_state = GameState::PLAYING;

            }

            if (selectedMode == 1) {
                App->map->SetMapTexture((selectedMap == 0) ? menuManager->GetMap1Full() : menuManager->GetMap2Full());
                iceMap = (selectedMap == 1);

                if (car1 != nullptr) {
                    car1->SetIceMap(iceMap);
                }
                if (car2 != nullptr) {
                    car2->SetIceMap(iceMap);
                }
                game_state = GameState::INTRO_REDGREEN;
            }

        }

        break;
    case GameState::INTRO_REDGREEN:
        menuManager->DrawInstructions();
        if (IsKeyPressed(KEY_ENTER)) {

            game_state = GameState::PLAYING_REDGREEN;

        }


        break;
    case GameState::PLAYING:


        SetMusicVolume(playingMusic, 0.15f);

        if (IsKeyPressed(KEY_Q)) {


            game_state = GameState::PAUSED;

        }

        car1->Draw();
        car2->Draw();



        trafficLight->Update();

        if (!trafficLight->IsCountdownFinished()) {
            trafficLight->Draw();
            return UPDATE_CONTINUE;
        }
        else if (!canControlCar) {
            canControlCar = true;
            lapStartTimeCar1 = GetTime();
            lapStartTimeCar2 = GetTime();
        }

        if (canControlCar)
        {

            App->map->Update();
            nitro->Update();
            nitro->Draw();
            oil->Draw();
            car1->Update();
            car1->Draw();
            car2->Update();
            car2->Draw();


            App->particleSystem->Update();
            UpdateLapTime();
            ui->Update(currentLapTimeCar1, bestLapTimeCar1, lapsCompletedCar1,
                currentLapTimeCar2, bestLapTimeCar2, lapsCompletedCar2);
            ui->Draw();


            if (IsKeyDown(KEY_UP))
            {
                car1->Accelerate();

            }

            if (IsKeyDown(KEY_DOWN))
            {
                car1->Brake();

            }

            if (IsKeyDown(KEY_SPACE))
            {
                car1->Nitro();
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

            if (IsKeyDown(KEY_W))
            {
                car2->Accelerate();
            }

            if (IsKeyDown(KEY_S))
            {
                car2->Brake();
            }

            if (IsKeyDown(KEY_A))
            {
                car2->Turn(-1, true);
            }
            else if (IsKeyDown(KEY_D))
            {
                car2->Turn(1, true);
            }
            else
            {
                car2->Turn(0, false);
            }
            if (IsKeyDown(KEY_E))
            {
                car2->Nitro();
            }


        }

        break;
    case GameState::PLAYING_REDGREEN:
        redGreen = true;
        SetMusicVolume(playingMusic, 0.07f);

        if (IsKeyPressed(KEY_Q)) {


            game_state = GameState::PAUSED;

        }

        car1->Draw();
        car2->Draw();



        trafficLight->Update();

        if (!trafficLight->IsCountdownFinished()) {
            trafficLight->Draw();
            return UPDATE_CONTINUE;
        }
        else if (!canControlCar) {
            canControlCar = true;
            lapStartTimeCar1 = GetTime();
            lapStartTimeCar2 = GetTime();
        }

        if (canControlCar)
        {

            App->map->Update();
            nitro->Update();
            nitro->Draw();
            oil->Draw();
            car1->Update();
            car1->Draw();
            car2->Update();
            car2->Draw();


            App->particleSystem->Update();
            UpdateLapTime();
            ui->Update(currentLapTimeCar1, bestLapTimeCar1, lapsCompletedCar1,
                currentLapTimeCar2, bestLapTimeCar2, lapsCompletedCar2);
            ui->Draw();


            if (IsKeyDown(KEY_UP))
            {
                car1->Accelerate();

            }

            if (IsKeyDown(KEY_DOWN))
            {
                car1->Brake();

            }

            if (IsKeyDown(KEY_SPACE))
            {
                car1->Nitro();
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

            if (IsKeyDown(KEY_W))
            {
                car2->Accelerate();
            }

            if (IsKeyDown(KEY_S))
            {
                car2->Brake();
            }

            if (IsKeyDown(KEY_A))
            {
                car2->Turn(-1, true);
            }
            else if (IsKeyDown(KEY_D))
            {
                car2->Turn(1, true);
            }
            else
            {
                car2->Turn(0, false);
            }
            if (IsKeyDown(KEY_E))
            {
                car2->Nitro();
            }


            static bool isRedLight = false;
            static bool initialSoundPlayed = false; 
            static float lastToggleTime = GetTime();
            const float toggleInterval = 5.0f;

            if (!isRedLight && !initialSoundPlayed) {
                PlaySound(red_light_fx); 
                initialSoundPlayed = true;
            }

            if (GetTime() - lastToggleTime >= toggleInterval) {
                isRedLight = !isRedLight; 
                lastToggleTime = GetTime();

                if (!isRedLight) {
                    PlaySound(red_light_fx); 
                }

            }

            if (isRedLight) {
                DrawTexture(red_light, 0, 0, WHITE);
            }
            else {
                DrawTexture(green_light, 0, 0, WHITE);
            }

            if (isRedLight) {
                if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) ||
                    IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S) ||
                    IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) ||
                    IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) ||
                    IsKeyDown(KEY_SPACE)) {
                    game_state = GameState::GAME_OVER;
                }
            }
        }
        break;



    case GameState::PAUSED:

        menuManager->DrawPauseMenu();

        PauseMusicStream(playingMusic);

        if (IsKeyPressed(KEY_ENTER) && redGreen == false) {

            game_state = GameState::PLAYING;

            PlayMusicStream(playingMusic);

        }
        else if (IsKeyPressed(KEY_ENTER) && redGreen == true) {

            game_state = GameState::PLAYING_REDGREEN;

            PlayMusicStream(playingMusic);

        }
        break;


    case GameState::WIN:

        menuManager->DrawWinMenu();

        if (IsKeyPressed(KEY_LEFT_SHIFT)) {

            lapsCompletedCar1 = 0;
            currentLapTimeCar1 = 0.0f;
            bestLapTimeCar1 = FLT_MAX;

            lapsCompletedCar2 = 0;
            currentLapTimeCar2 = 0.0f;
            bestLapTimeCar2 = FLT_MAX;

            ResetCheckpoints();
            StopSound(victory_fx);
            PlayMusicStream(playingMusic);
            trafficLight->StartCountdown(3.0f);
            canControlCar = false;
            game_state = GameState::SELECT_CHARACTER_MENU;
        }
        break;
    }
    if (game_state == GameState::GAME_OVER)
    {
        menuManager->DrawGameOverMenu();
        if (IsKeyPressed(KEY_LEFT_SHIFT)) {
            if (car1 != nullptr)
            {
                delete car1;
                car1 = nullptr;
            }
            if (car2 != nullptr)
            {
                delete car2;
                car2 = nullptr;
            }

            lapsCompletedCar1 = 0;
            currentLapTimeCar1 = 0.0f;
            bestLapTimeCar1 = FLT_MAX;

            lapsCompletedCar2 = 0;
            currentLapTimeCar2 = 0.0f;
            bestLapTimeCar2 = FLT_MAX;

            ResetCheckpoints();
            StopSound(victory_fx);
            PlayMusicStream(playingMusic);
            trafficLight->StartCountdown(3.0f);
            canControlCar = false;
            game_state = GameState::SELECT_CHARACTER_MENU;
        }

    }
    return UPDATE_CONTINUE;


}


void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
    if (bodyA != nullptr && bodyB != nullptr)
    {
        Car* collidingCar = nullptr;
        int* currentCheckpointIndex = nullptr;
        int* lapsCompleted = nullptr;
        float* lapStartTime = nullptr;
        float* bestLapTime = nullptr;
        std::vector<bool>* checkpointsActive = nullptr;

        if (bodyA->entity == car1 || bodyB->entity == car1)
        {
            collidingCar = car1;
            currentCheckpointIndex = &currentCheckpointIndexCar1;
            lapsCompleted = &lapsCompletedCar1;
            lapStartTime = &lapStartTimeCar1;
            bestLapTime = &bestLapTimeCar1;
            checkpointsActive = &checkpointsActiveCar1;
        }
        else if (bodyA->entity == car2 || bodyB->entity == car2)
        {
            collidingCar = car2;
            currentCheckpointIndex = &currentCheckpointIndexCar2;
            lapsCompleted = &lapsCompletedCar2;
            lapStartTime = &lapStartTimeCar2;
            bestLapTime = &bestLapTimeCar2;
            checkpointsActive = &checkpointsActiveCar2;
        }

        if (collidingCar != nullptr)
        {
            if (bodyB->colliderType == ColliderType::NITRO)
            {
                Nitro* nitro = static_cast<Nitro*>(bodyB->entity);
                if (nitro && nitro->isAvailable())
                {
                    nitro->OnPlayerCollision();
                    PlaySound(bonus_fx);
                    collidingCar->ApplyBoost(15.0f);

                    b2Vec2 carPosition = collidingCar->body->body->GetPosition();
                    Vector2 position = {
                        carPosition.x * PIXELS_PER_METER,
                        carPosition.y * PIXELS_PER_METER
                    };
                    App->particleSystem->SpawnParticles(position, ParticleType::NITRO);
                }
            }

            if (bodyB->colliderType == ColliderType::OIL)
            {
                if (oil && !collidingCar->oilCooldownActive)
                {
                    oil->OnPlayerCollision();
                    PlaySound(oil_fx);

                    b2Vec2 carPosition = collidingCar->body->body->GetPosition();
                    Vector2 position = {
                        carPosition.x * PIXELS_PER_METER,
                        carPosition.y * PIXELS_PER_METER
                    };
                    App->particleSystem->SpawnParticles(position, ParticleType::SMOKE);

                    collidingCar->isSpinning = true;
                    collidingCar->spinningTimeLeft = collidingCar->spinningDuration;

                    b2Vec2 velocity = collidingCar->body->body->GetLinearVelocity();
                    if (velocity.Length() > 0.0f)
                    {
                        collidingCar->preSpinDirection = velocity;
                        collidingCar->preSpinDirection.Normalize();
                    }
                    else
                    {
                        collidingCar->preSpinDirection.SetZero();
                    }

                    collidingCar->body->body->SetLinearVelocity(b2Vec2(0, 0));
                    collidingCar->oilCooldownActive = true;
                    collidingCar->oilCooldownTimeLeft = collidingCar->oilCooldownDuration;
                }
            }

            if (collidingCar != nullptr && bodyB->colliderType == ColliderType::CHECKPOINT)
            {
                Checkpoint* checkpoint = static_cast<Checkpoint*>(bodyB->entity);
                if (checkpoint)
                {
                    HandleCheckpointForCar(checkpoint, *currentCheckpointIndex, *lapsCompleted, *lapStartTime, *bestLapTime, *checkpointsActive);
                }
            }
        }
    }
}

void ModuleGame::HandleCheckpointForCar(Checkpoint* checkpoint, int& currentCheckpointIndex, int& lapsCompleted, float& lapStartTime, float& bestLapTime, std::vector<bool>& checkpointsActive)
{
    if (checkpoint->index == 0)
    {
        bool allCheckpointsActive = std::all_of(checkpointsActive.begin(), checkpointsActive.end(), [](bool active) { return active; });

        if (allCheckpointsActive)
        {
            float currentLapTime = GetTime() - lapStartTime;

            if (bestLapTime == 0.0f || currentLapTime < bestLapTime)
            {
                bestLapTime = currentLapTime;
                LOG("New best lap time: %.2f seconds", bestLapTime);
            }

            lapStartTime = GetTime();
            lapsCompleted++;
            PlaySound(finish_line_fx);

            ResetCheckpointsForCar(checkpointsActive, currentCheckpointIndex);

            LOG("Lap completed! Total laps: %d", lapsCompleted);
            LOG("Current lap time: %.2f seconds", currentLapTime);

            if (lapsCompleted >= totalLaps)
            {
                PauseMusicStream(playingMusic);
                PlaySound(victory_fx);
                game_state = GameState::WIN;
                LOG("WIN Active");
            }
        }

        checkpointsActive[checkpoint->index] = true;
        currentCheckpointIndex = 1;
    }
    else if (checkpoint->index == currentCheckpointIndex)
    {
        checkpointsActive[checkpoint->index] = true;
        currentCheckpointIndex++;
    }
}

void ModuleGame::ResetCheckpointsForCar(std::vector<bool>& checkpointsActive, int& currentCheckpointIndex)
{
    std::fill(checkpointsActive.begin(), checkpointsActive.end(), false);
    currentCheckpointIndex = 0;
}


void ModuleGame::ResetCheckpoints()
{
    ResetCheckpointsForCar(checkpointsActiveCar1, currentCheckpointIndexCar1);
    ResetCheckpointsForCar(checkpointsActiveCar2, currentCheckpointIndexCar2);
}


void ModuleGame::UpdateLapTime()
{
    currentLapTimeCar1 = GetTime() - lapStartTimeCar1;
    currentLapTimeCar2 = GetTime() - lapStartTimeCar2;
}
