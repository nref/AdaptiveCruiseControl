#pragma once

// Sources
// http://www.codeproject.com/Articles/36459/PID-process-control-a-Cruise-Control-example
// http://itech.fgcu.edu/faculty/zalewski/cda4170/files/pidcontrol.pdf
// http://www.cse.iitb.ac.in/~erts/car/documents/presentations/ashish-stage1.pdf
class pidController
{
public:
	pidController()
	{
		setpoint = 0;
		PV = 0;
		output = 0;
		noise = 0;
		error = 0;
		preError = 0;
		integral = 0;
		derivative = 0;

		Ki = 0;
		Kd = 0;
		Kp = 0;
	}

	// Tick the process value simulation
	// Should be significantly faster tick interval than pidTick()
	void pvTick()
	{
		// As a rule of thumb:
		// ProcessValue = ProcessValue + (output * efficiency) – loss
		
		// 20% efficient engine, 10% energy loss at a given speed
		PV = PV + (output * 0.20) - (PV * 0.10) + noise;
	}

	// Tick the PID controller simulation
	// Should be significantly slower tick interval than pvTick()
	void pidTick(const double& dt)
	{
		error = setpoint - PV; 
		// track error over time, scaled to the timer interval
		integral = integral + (error * dt);
		// determine the amount of change from the last time checked
		derivative = (error - preError) / dt; 
		// calculate how much to drive the output in order to get to the 
		// desired setpoint. 
		output = (Kp * error) + (Ki * integral) + (Kd * derivative);
		// remember the error for the next time around.
		preError = error; 
	}

	double getKi() const
	{
		return Ki;
	}

	void setKi(const double& value)
	{
		Ki = value;
	}

	double getKd() const
	{
		return Kd;
	}

	void setKd(const double& value)
	{
		Kd = value;
	}

	double getKp() const
	{
		return Kp;
	}

	void setKp(const double& value)
	{
		Kp = value;
	}

	void setSetPoint(const double& value)
	{
		setpoint = value;
	}

	double getSetPoint() const
	{
		return setpoint;
	}

	/// Members with only getters below

	double getProcessValue() const
	{
		return PV;
	}

	double getError() const
	{
		return error;
	}

	double getIntegral() const
	{
		return integral;
	}

	double getDerivative() const
	{
		return derivative;
	}

	double getOutput() const
	{
		return output;
	}

private:

	// The value PV will try to reach
	double setpoint;

	// Process value
	double PV;

	// Process output
	double output;

	// Signal noise
	// TODO set with random distribution
	// Set at high frequency to simulate electrical noise
	// set at low frequency to simulate hills, wind
	double noise;

	double error;
	double preError;
	double integral;
	double derivative;

	double Ki;
	double Kd;
	double Kp;
};