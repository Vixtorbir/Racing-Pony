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
    // Background for UI (left block)
    DrawRectangleRounded(Rectangle{ 5, 30, 230, 75 }, 0.2f, 10, Fade(BLACK, 0.8f)); // Smaller rounded rectangle for left block
  
    // Text for laps
    Vector2 lapsPosition = { 12, 40 }; // Adjusted position for left UI
    DrawTextEx(font, TextFormat("Laps: %d / %d", lapsCompleted, totalLaps), lapsPosition, 24, 0, WHITE);

    // Text for current lap time
    Vector2 currentLapPosition = { 12, 75 }; // Adjusted position
    Color lapColor = (currentLapTime < bestLapTime) ? Color{ 144, 238, 144, 255 } : WHITE; // Pastel green
    DrawTextEx(font, TextFormat("Current Lap: %.2f s", currentLapTime), currentLapPosition, 22, 0, lapColor);

    // Text for best lap time
    if (bestLapTime < FLT_MAX) {
        DrawRectangleRounded(Rectangle{ 1055, 30, 210, 38 }, 0.2f, 10, Fade(BLACK, 0.8f));
        Vector2 bestLapPosition = { 1062, 38 }; // Moved further to the right
        Color bestLapColor = Color{ 218, 112, 214, 255 }; // Purple with border
        DrawTextEx(font, TextFormat("Best Lap: %.2f s", bestLapTime), bestLapPosition, 24, 0, bestLapColor);
    }
}





