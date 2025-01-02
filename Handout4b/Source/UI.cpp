#include "UI.h"
#include <cfloat>

UI::UI(int totalLaps) : currentLapTime(0.0f), bestLapTime(FLT_MAX), lapsCompleted(0), totalLaps(totalLaps) 
{

    font = LoadFont("Assets/myfont.ttf");
}

UI::~UI() {}

void UI::Update(float currentLapTime, float bestLapTime, int lapsCompleted) {
    this->currentLapTime = currentLapTime;
    this->bestLapTime = bestLapTime;
    this->lapsCompleted = lapsCompleted;
}

void UI::Draw() {

    DrawTextEx(font, TextFormat("Laps: %d / %d", lapsCompleted, totalLaps), { 340, 20 }, 30, 0, BLACK);
   
	DrawTextEx(font, TextFormat("Current Lap: %.2f s", currentLapTime), { 490, 20 }, 30, 0, BLACK);

    if (bestLapTime < FLT_MAX) {
       
		DrawTextEx(font, TextFormat("Best Lap: %.2f s", bestLapTime), { 790, 20 }, 30, 0, BLACK);
    }
}
