#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Car.h"
#include "Map.h" 



ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled), selectedCharacter(0), iceMap(false)
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
   
    menuManager = new MenuManager();

    menuManager->LoadAssets();

    selectedCharacter = 0;
    selectedMap = 0;

    nitro = new Nitro(App->physics->CreateRectangleSensor(200, 300, 20,20), LoadTexture("Assets/nitro.png"),this);

	oil = new OilSlick(App->physics->CreateCircleSensor(400, 580, 15), LoadTexture("Assets/stain.png"), this);

    green_light = LoadTexture("Assets/GREENLIGHT.png");
    red_light = LoadTexture("Assets/REDLIGHT.png");
 
    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(440, 115, 10, 90), 0));
    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(750, 433, 10, 80), 1));
    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(550, 600, 10, 90), 2));
    checkpoints.push_back(new Checkpoint(App->physics->CreateRectangleSensor(235, 340, 92, 10), 3)); 


    bonus_fx= LoadSound("Assets/music/bonus_sfx.wav");
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
  
	if (lapsCompletedCar1 >= totalLaps || lapsCompletedCar2 >= totalLaps){
        LOG("Juego completado");

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

        if (IsKeyPressed(KEY_RIGHT)) selectedCharacter = 1;
        if (IsKeyPressed(KEY_LEFT)) selectedCharacter = 0;

        if (IsKeyPressed(KEY_ENTER)) {

            Texture2D carTexture = (selectedCharacter == 0) ? menuManager->GetCharacter1Texture() : menuManager->GetCharacter2Texture();
			Texture2D carTexture2 = (selectedCharacter == 1) ? menuManager->GetCharacter1Texture() : menuManager->GetCharacter2Texture();
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
        menuManager->DrawPauseMenu();
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

            //RedLightGreenLight

            static bool isRedLight = false;
            static bool initialSoundPlayed = false; // Tracks if the initial sound has been played
            static float lastToggleTime = GetTime();
            const float toggleInterval = 5.0f;

            // Ensure the initial green light sound plays once
            if (!isRedLight && !initialSoundPlayed) {
                PlaySound(red_light_fx); // Replace with your green light sound
                initialSoundPlayed = true;
            }

            // Toggle traffic light based on time
            if (GetTime() - lastToggleTime >= toggleInterval) {
                isRedLight = !isRedLight; // Switch light state
                lastToggleTime = GetTime();

                if (!isRedLight) {
                    PlaySound(red_light_fx); // Replace with your red light sound
                }

            }

            // Display traffic light status
            if (isRedLight) {
                DrawTexture(red_light, 0, 0, WHITE);
            }
            else {
                DrawTexture(green_light, 0, 0, WHITE);
            }

            // Movement detection during red light
            if (isRedLight) {
                if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) ||
                    IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S) ||
                    IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) ||
                    IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) ||
                    IsKeyDown(KEY_SPACE)) {
                    // Penalize the player for moving during red light
                    game_state = GameState::GAME_OVER;
                }
            }
        }
        break;



    case GameState::PAUSED:

        menuManager->DrawPauseMenu();

        PauseMusicStream(playingMusic);

        if (IsKeyPressed(KEY_ENTER)) {

            //PlaySound(el que sea);
            game_state = GameState::PLAYING;

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

        // Identificar qué coche está involucrado
        if (bodyA->entity == car1) {
            collidingCar = car1;
        }
        else if (bodyA->entity == car2) {
            collidingCar = car2;
        }

        if (bodyB->entity == car1) {
            collidingCar = car1;
        }
        else if (bodyB->entity == car2) {
            collidingCar = car2;
        }

        if (collidingCar != nullptr) {
            // Aquí manejas la colisión específica del coche
            if (bodyB->colliderType == ColliderType::NITRO) {
                Nitro* nitro = static_cast<Nitro*>(bodyB->entity);
                if (nitro && nitro->isAvailable()) {
                    nitro->OnPlayerCollision();
                    PlaySound(bonus_fx);
                    collidingCar->ApplyBoost(15.0f); // Solo afecta al coche que colisiona

                    b2Vec2 carPosition = collidingCar->body->body->GetPosition();
                    Vector2 position = {
                        carPosition.x * PIXELS_PER_METER,
                        carPosition.y * PIXELS_PER_METER
                    };
                    App->particleSystem->SpawnParticles(position, ParticleType::NITRO);
                }
            }

            if (bodyB->colliderType == ColliderType::OIL) {
                if (oil && !collidingCar->oilCooldownActive) {
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
                    if (velocity.Length() > 0.0f) {
                        collidingCar->preSpinDirection = velocity;
                        collidingCar->preSpinDirection.Normalize();
                    }
                    else {
                        collidingCar->preSpinDirection.SetZero();
                    }

                    collidingCar->body->body->SetLinearVelocity(b2Vec2(0, 0));
                    collidingCar->oilCooldownActive = true;
                    collidingCar->oilCooldownTimeLeft = collidingCar->oilCooldownDuration;
                }
            }
        }

        if (collidingCar != nullptr && bodyB->colliderType == ColliderType::CHECKPOINT) {
            Checkpoint* checkpoint = static_cast<Checkpoint*>(bodyB->entity);
            if (checkpoint) {
                if (collidingCar == car1) {
                    HandleCheckpointForCar(checkpoint, currentCheckpointIndexCar1, lapsCompletedCar1, lapStartTimeCar1, bestLapTimeCar1);
                }
                else if (collidingCar == car2) {
                    HandleCheckpointForCar(checkpoint, currentCheckpointIndexCar2, lapsCompletedCar2, lapStartTimeCar2, bestLapTimeCar2);
                }
            }
        }
    }
}

void ModuleGame::HandleCheckpointForCar(Checkpoint* checkpoint, int& currentCheckpointIndex, int& lapsCompleted, float& lapStartTime, float& bestLapTime)
{
    if (checkpoint->index == 0) {
        // Verificar si todos los checkpoints fueron activados
        bool allCheckpointsActive = true;
        for (Checkpoint* cp : checkpoints) {
            if (!cp->isActive) {
                allCheckpointsActive = false;
                break;
            }
        }

        if (allCheckpointsActive) {
            // Calcular el tiempo de vuelta
            float currentLapTime = GetTime() - lapStartTime;

            if (bestLapTime == 0.0f || currentLapTime < bestLapTime) {
                bestLapTime = currentLapTime;
                LOG("New best lap time: %.2f seconds", bestLapTime);
            }

            lapStartTime = GetTime();
            lapsCompleted++;
            PlaySound(finish_line_fx); // Cambiar audio si es necesario
            ResetCheckpoints();
            LOG("Lap completed! Total laps: %d", lapsCompleted);
            LOG("Current lap time: %.2f seconds", currentLapTime);

            if (lapsCompleted >= totalLaps) {
                PauseMusicStream(playingMusic);
                PlaySound(victory_fx);
                game_state = GameState::WIN;
                LOG("WIN Active");
            }
        }

        checkpoint->isActive = true;
        currentCheckpointIndex = 1;
    }
    else if (checkpoint->index == currentCheckpointIndex) {
        checkpoint->isActive = true;
        currentCheckpointIndex++;
    }
}


void ModuleGame::ResetCheckpoints()
{
    currentCheckpointIndexCar1 = 0;
    currentCheckpointIndexCar2 = 0;

    for (Checkpoint* checkpoint : checkpoints) {
        checkpoint->isActive = false;
    }
}

void ModuleGame::UpdateLapTime() {
    currentLapTimeCar1 = GetTime() - lapStartTimeCar1;
    currentLapTimeCar2 = GetTime() - lapStartTimeCar2;
}




