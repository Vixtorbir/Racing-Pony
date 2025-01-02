#pragma once
#include "raylib.h"

class TrafficLight {
public:
    TrafficLight();
    ~TrafficLight();

    void StartCountdown(float duration);
    void Update();
    void Draw();

    bool IsCountdownFinished() const;

private:
    float countdownTime; 
    float timeRemaining; 
    bool countdownActive;
};
