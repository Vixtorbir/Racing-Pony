#include "TrafficLight.h"
#include <cmath>

TrafficLight::TrafficLight() : countdownTime(0), timeRemaining(0), countdownActive(false) {}

TrafficLight::~TrafficLight() {}

void TrafficLight::StartCountdown(float duration) {
    countdownTime = duration;
    timeRemaining = duration;
    countdownActive = true;
}

void TrafficLight::Update() {
    if (countdownActive) {
        timeRemaining -= GetFrameTime();
        if (timeRemaining <= -1.0f) { 
            countdownActive = false;
        }
    }
}

void TrafficLight::Draw() {
    if (countdownActive || timeRemaining > -1.0f) { 
        const char* text = nullptr;
        if (timeRemaining > 0) {
            int displayTime = static_cast<int>(ceil(timeRemaining));
            text = TextFormat("%d", displayTime); 
        }
        else {
            text = "GO!"; 
        }

        int fontSize = 120;
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 2);
        DrawText(text, (GetScreenWidth() - textSize.x) / 2, (GetScreenHeight() - textSize.y) / 2, fontSize, BLACK);
    }
}

bool TrafficLight::IsCountdownFinished() const {
    return !countdownActive;
}
