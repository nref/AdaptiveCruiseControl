#include "car.h"
#include <iostream>

int car::idGlobal = 0;

void car::stepSim(double dt)
{
	position += speed * dt;
}