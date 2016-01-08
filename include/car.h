#pragma once

#include <string>

class car
{
public:

	car(const std::string& carName = "")
	{
		name = carName;
		id = idGlobal++;
	}

	void setSpeed(const double& newSpeed)
	{
		speed = newSpeed;
	}

	double getSpeed() const
	{
		return speed;
	}

	void stepSim(double dt);

	double getPosition() const
	{
		return position;
	}

	void setPosition(const double& newPosition)
	{
		position = newPosition;
	}

	std::string getName() const
	{
		return name;
	}

	void setName(const std::string& value)
	{
		name = value;
	}

	int getId() const
	{
		return id;
	}

	void setId(const int& value)
	{
		id = value;
	}

private:

	int id;
	static int idGlobal;

	std::string name;

	double speed;
	double position;
};