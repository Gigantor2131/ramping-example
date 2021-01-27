#include "main.h"
#include "T_curve.hpp"
#include <memory>
using namespace okapi;
using pros::delay;
using std::abs;

T_curve profile(2,10,200,-200);

profile.request(5);
profile.calculate();
profile.getOutput();


void initialize() {}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

#define motorMaxRPM 200
#define motorMinRPM -200

double yDistance();
double pY;

double vY, vX, vZ;

std::shared_ptr<AbstractMotor> left;
void driveY(double pTarget, double vRequest, double a, int endwait = 0, double vStop = 0)
{
	T_curve profile(a, motorUpdateRate, motorMaxRPM, motorMinRPM);
	profile.instant(0);											//incase motor isnt at full stop //todo move profile to global/longer lifespan
	double distanceUntilStop = profile.integrateToValue(vStop); //most times this will be 0

	//phase 1
	profile.request(vRequest); //request the velocity

	//positionTarget - current Y position - distance reqired to come to stop
	while (yDistance() < pTarget - profile.integrateToValue(vStop))
	{
		vY = profile.calculate(); //calculate velocity
		// pYUpdate();
		std::cout << "pY, " << yDistance() << " ,vY, " << vY;
		distanceUntilStop = profile.integrateToValue(vStop); //calculate stopping distance
		std::cout << " ,pS, " << distanceUntilStop << std::endl;
		// delay(profile.getChangeMs());						 //delay until next time slice/tick

		//velocity is maxed out, no need to calculate an updated velocity, or stoping distance
		if (vY == vRequest)
			break;
	}
	std::cout << "phase 2" << std::endl;
	//phase 2
	while (yDistance() < pTarget - profile.integrateToValue(vStop))
	{
		// pYUpdate();
		std::cout << "pY, " << yDistance() << " ,vY, " << vY;
		std::cout << " ,pS, " << distanceUntilStop << std::endl;

		delay(profile.getChangeMs()); //delay until next time slice/tick
	}

	std::cout << "phase 3" << std::endl;
	//phase 3
	profile.request(0);
	while (profile.getOutput() != vStop) //if left with just this stopping is an open loop AKA no encoder/sensor verification. which is bad, however this is how all of okapi's motor profiles get executed to my knowledge so atleast its a step up
	//i would recommed tunning (by printing out all values in csv format and comparing them in some graphics) the profile until there is little error in calculated VS actual profile values(p,v,a...)
	{
		double projectedStopDelta = (yDistance() + distanceUntilStop) - pTarget;
		double projectedStopTol = 0.25;
		std::cout << "pY, " << yDistance() << " ,vY, " << vY << " ,pS, " << projectedStopDelta << std::endl;

		//error minimizing stop
		if (projectedStopDelta < projectedStopTol)
		{
			//if projected to be short

			/*option 1
			dont reset encoders and allow for next movement to correct this ones stopping error
			*/

			/*option 2
			 slow down slower
			 AKA decrese acc so the dirve travels more distance while slowing down
			*/
			// profile.setChangeValue(profile.getChangeValue() - 1);

			/*option 3
			 allow for one extra loop to take place
			 AKA dont calculate a slower velocity
			*/
		}
		if (projectedStopDelta > projectedStopTol)
		{
			//if projected to be long

			/*option 1
			dont reset encoders and allow for next movement to correct this ones stopping error
			*/

			/*option 2
			 slow down faster
			 AKA increse acc so the dirve travels less distance while slowing down
			 //! note only do this if your profile can handle it!!!, 
			 //! this would require your profile to have head room to slow down faster if needed.
			 //! however that might work okay with idle tracking wheels as powered wheels can slip during a worse case event
			*/
			// profile.setChangeValue(profile.getChangeValue() + 1);
		}

		vY = profile.calculate();							 //calculate velocity
		distanceUntilStop = profile.integrateToValue(vStop); //calculate stopping distance
		// pYUpdate();
		delay(profile.getChangeMs()); //allow for motors to move
	}
}

void opcontrol()
{
	T_curve profile(1, motorUpdateRate, 200, -200);

	profile.request(200);
	while (profile.getOutput() != 200)
	{ //ramps up to 200 RPM
		left->moveVelocity(profile.calculate());
		delay(motorUpdateRate);
	}
}
