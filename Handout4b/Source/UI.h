#pragma once

#include "raylib.h"

class UI {
public:
    UI(int totalLaps);
    ~UI();

    void Update(float currentLapTime, float bestLapTime, int lapsCompleted);
    void Draw();

private:
    float currentLapTime;
    float bestLapTime;
    int lapsCompleted;
    int totalLaps;
};
