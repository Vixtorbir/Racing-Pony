#pragma once

#include "raylib.h"

class UI {
public:
    UI(int totalLaps);
    ~UI();

	void Update(float currentLapTimeCar1, float bestLapTimeCar1, int lapsCompletedCar1, float currentLapTimeCar2, float bestLapTimeCar2, int lapsCompletedCar2);
    void Draw();

private:
   
	float currentLapTimeCar1;
	float bestLapTimeCar1;
	int lapsCompletedCar1;

	float currentLapTimeCar2;
	float bestLapTimeCar2;
	int lapsCompletedCar2;

    int totalLaps;
	Font font;
};
