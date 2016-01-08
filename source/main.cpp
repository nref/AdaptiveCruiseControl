#include "cruiseController.h"
#include "accController.h"
#include "car.h"

#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>
#include <cmath>
#include <random>
#include <signal.h>

bool quit;
double pvTickResolutionSeconds = 0.016667; // ~= 1/60s 
double pidTickResolutionSeconds = .001;
double simTickResolutionSeconds = .01;

std::mutex coutMutex;

void handleSignal(int s)
{
	std::lock_guard<std::mutex> lock(coutMutex);
	quit = true;
}

void sleepSim(double seconds)
{
	std::this_thread::sleep_for
	(
		std::chrono::milliseconds((int)(1000*seconds))
	);
}

void runPv(pidController* controller)
{
	while (!quit)
	{
		controller->pvTick();
		sleepSim(pvTickResolutionSeconds);
	}
}

void runPid(pidController* controller)
{
	while (!quit)
	{

		controller->pidTick(pidTickResolutionSeconds);
		sleepSim(pidTickResolutionSeconds);
	}
}

void runSim(car* lead, car* follower, pidController* controller)
{
	double t = 0;

	std::cout << "time, lead distance, setpoint, PV, integral, derivative, error\n";
	while (!quit)
	{
		t += simTickResolutionSeconds;

		std::string delim = ", ";
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cout 
				<< t
				<< delim << lead->getPosition() - follower->getPosition()
				<< delim << controller->getSetPoint()
				<< delim << controller->getProcessValue()
				<< delim << controller->getIntegral()
				<< delim << controller->getDerivative()
				<< delim << controller->getError()
				<< "\n";
		}

		lead->stepSim(simTickResolutionSeconds);
		follower->stepSim(simTickResolutionSeconds);
		follower->setSpeed(controller->getProcessValue());

		sleepSim(simTickResolutionSeconds);
	}
}

int main()
{
	// Handle CTLR + C
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = handleSignal;
	sigemptyset(&sigIntHandler.sa_mask);
	sigaction(SIGINT, &sigIntHandler, NULL);

	quit = false;
	auto lead = new car("lead");
	auto follower = new car("follower");
	auto controller = new pidController();

	follower->setSpeed(0);
	lead->setSpeed(0);
	controller->setSetPoint(lead->getSpeed());

	controller->setKi(1);
	controller->setKd(1);
	controller->setKp(.2);

	auto pvThread = std::thread(runPv, controller);
	auto pidThread = std::thread(runPid, controller);
	auto simThread = std::thread(runSim, lead, follower, controller);

	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(0,4); // guaranteed unbiased

	while (!quit)
	{
		int randomSpeed = uni(rng);

		lead->setSpeed(randomSpeed);
		controller->setSetPoint(lead->getSpeed());

		sleepSim(2);
	}

	pvThread.join();
	simThread.join();
	pidThread.join();

	delete lead;
	delete follower;
	delete controller;

	return 0;
}