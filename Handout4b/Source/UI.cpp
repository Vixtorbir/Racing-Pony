#include "UI.h"
#include <cfloat>

UI::UI(int totalLaps) : currentLapTime(0.0f), bestLapTime(FLT_MAX), lapsCompleted(0), totalLaps(totalLaps) 
{

    font = LoadFont("Assets/Fonts/myfont.ttf");
}

UI::~UI() {}

void UI::Update(float currentLapTime, float bestLapTime, int lapsCompleted) {
    this->currentLapTime = currentLapTime;
    this->bestLapTime = bestLapTime;
    this->lapsCompleted = lapsCompleted;
}

void UI::Draw() {
	// Dibujar información de vueltas con fuente personalizada

    DrawTextEx(font, TextFormat("Laps: %d / %d", lapsCompleted, totalLaps), { 20, 20 }, 20, 0, WHITE);
   

	// Dibujar tiempo actual de vuelta con fuente personalizada

	DrawTextEx(font, TextFormat("Current Lap: %.2f s", currentLapTime), { 20, 50 }, 20, 0, WHITE);

	// Dibujar el mejor tiempo de vuelta con fuente personalizada

    if (bestLapTime < FLT_MAX) {
       
		DrawTextEx(font, TextFormat("Best Lap: %.2f s", bestLapTime), { 20, 80 }, 20, 0, WHITE);
    }
}
