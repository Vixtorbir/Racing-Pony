#include "TrafficLight.h"
#include <corecrt_math.h>

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
        if (timeRemaining <= 0) {
            countdownActive = false;
        }
    }
}

void TrafficLight::Draw() {
    if (countdownActive) {
        int displayTime = static_cast<int>(ceil(timeRemaining));
        const char* text = (displayTime > 0) ? TextFormat("%d", displayTime) : "GO!";
        int fontSize = 100;
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 2);
        DrawText(text, (GetScreenWidth() - textSize.x) / 2, (GetScreenHeight() - textSize.y) / 2, fontSize, BLACK);
    }
}

bool TrafficLight::IsCountdownFinished() const {
    return !countdownActive;
}
