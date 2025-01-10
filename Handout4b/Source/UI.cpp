#include "UI.h"
#include <cfloat>

UI::UI(int totalLaps) : currentLapTimeCar1(0.0f), bestLapTimeCar1(FLT_MAX), lapsCompletedCar1(0),
currentLapTimeCar2(0.0f), bestLapTimeCar2(FLT_MAX), lapsCompletedCar2(0), totalLaps(totalLaps)
{
    font = LoadFont("Assets/myfont.ttf");
}

UI::~UI() {}


void UI::Update(float currentLapTimeCar1, float bestLapTimeCar1, int lapsCompletedCar1, float currentLapTimeCar2, float bestLapTimeCar2, int lapsCompletedCar2)
{
    this->currentLapTimeCar1 = currentLapTimeCar1;
    this->bestLapTimeCar1 = bestLapTimeCar1;
    this->lapsCompletedCar1 = lapsCompletedCar1;
    this->currentLapTimeCar2 = currentLapTimeCar2;
    this->bestLapTimeCar2 = bestLapTimeCar2;
    this->lapsCompletedCar2 = lapsCompletedCar2;
}

void UI::Draw() {
    
    DrawRectangleRounded(Rectangle{ 5, 30, 220, 75 }, 0.2f, 10, Fade(BLACK, 0.8f)); 

   
    Vector2 lapsPosition = { 12, 35 };
    DrawTextEx(font, TextFormat("Laps: %d / %d", lapsCompletedCar1, totalLaps), lapsPosition, 20, 0, WHITE);

   
    Vector2 currentLapPosition = { 12, 59 };
    DrawTextEx(font, TextFormat("Current Lap: %.2f s", currentLapTimeCar1), currentLapPosition, 18, 0, Color{ 144, 238, 144, 255 }); // Green

   
    if (bestLapTimeCar1 < FLT_MAX) {
        Vector2 bestLapPosition = { 12,80 };
        DrawTextEx(font, TextFormat("Best Lap: %.2f s", bestLapTimeCar1), bestLapPosition, 20, 0, Color{ 218, 112, 214, 255 }); // Purple
    }

    // Background for UI (right block for Car 2) - moved to the bottom of the screen
    DrawRectangleRounded(Rectangle{ 5, 630, 220, 75 }, 0.2f, 10, Fade(BLACK, 0.8f)); // Car 2 block at the bottom of the screen

    // Text for laps (Car 2) - white
    Vector2 lapsPosition2 = { 12, 635 }; // Moved down to be at the bottom of the screen
    DrawTextEx(font, TextFormat("Laps: %d / %d", lapsCompletedCar2, totalLaps), lapsPosition2, 20, 0, WHITE);

    // Text for current lap time (Car 2) - green
    Vector2 currentLapPosition2 = { 12, 660 }; // Moved down to be at the bottom of the screen
    DrawTextEx(font, TextFormat("Current Lap: %.2f s", currentLapTimeCar2), currentLapPosition2, 18, 0, Color{ 144, 238, 144, 255 }); // Green

    // Text for best lap time (Car 2) - purple
    if (bestLapTimeCar2 < FLT_MAX) {
        Vector2 bestLapPosition2 = { 12, 681 }; // Moved down to be at the bottom of the screen
        DrawTextEx(font, TextFormat("Best Lap: %.2f s", bestLapTimeCar2), bestLapPosition2, 20, 0, Color{ 218, 112, 214, 255 }); // Purple
    }
}
