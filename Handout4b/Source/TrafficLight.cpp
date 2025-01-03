#include "TrafficLight.h"
#include <cmath>
#include "raylib.h" // Asegúrate de incluir Raylib para gestionar los sonidos

TrafficLight::TrafficLight() : countdownTime(0), timeRemaining(0), countdownActive(false) {}

TrafficLight::~TrafficLight() {
   

}

void TrafficLight::Initialize() {
 
    beepSound = LoadSound("Assets/music/bonus_sfx.wav");
    goSound = LoadSound("Assets/music/oil_sfx.wav");
}

void TrafficLight::StartCountdown(float duration) {
    countdownTime = duration;
    timeRemaining = duration;
    countdownActive = true;
}

void TrafficLight::Update() {
    if (countdownActive) {
        float previousTimeRemaining = timeRemaining; // Tiempo restante previo
        timeRemaining -= GetFrameTime();

        // Calcular el número entero actual para la cuenta atrás
        int currentDisplayTime = static_cast<int>(ceil(timeRemaining));

        // Reproducir sonido cuando el número entero visible cambia
        if (currentDisplayTime != lastDisplayedNumber && currentDisplayTime > 0) {
            PlaySound(beepSound);
            lastDisplayedNumber = currentDisplayTime; // Actualizar el número mostrado
        }

        // Reproducir sonido "GO!" cuando llegue a 0
        if (timeRemaining <= 0 && previousTimeRemaining > 0) {
            PlaySound(goSound);
        }

        // Finalizar la cuenta atrás si pasa el tiempo de "GO!"
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

        int fontSize = 320;
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 2);
        DrawText(text, (GetScreenWidth() - textSize.x) / 2, (GetScreenHeight() - textSize.y) / 2, fontSize, BLACK);
    }
}

bool TrafficLight::IsCountdownFinished() const {
    return !countdownActive;
}
