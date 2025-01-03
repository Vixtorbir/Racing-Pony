#pragma once
#include "raylib.h"

class TrafficLight {
public:
    TrafficLight();
    ~TrafficLight();

    void Initialize();

    void StartCountdown(float duration);
    void Update();
    void Draw();

    bool IsCountdownFinished() const;

private:
    float countdownTime; 
    float timeRemaining; 
    bool countdownActive;

	Sound beepSound;
	Sound goSound;

    int lastDisplayedNumber =-1;

};
