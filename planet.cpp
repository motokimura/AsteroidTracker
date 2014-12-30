#include "planet.h"

const double Planet:: MueSun_ = 1.32712442099e20;
const double Planet:: AU_ = 149597870700.0;

Planet:: Planet (void)
{
	//
}

Planet:: ~Planet (void)
{
	//
}

void Planet:: setOrbitInfo (OrbitInfo const& orbit_info)
{
	orbitInfo_.epochJDay = orbit_info.epochJDay;

	orbitInfo_.keplerian.a = orbit_info.keplerian.a * AU_;
	orbitInfo_.keplerian.e = orbit_info.keplerian.e;
	orbitInfo_.keplerian.i = orbit_info.keplerian.i * (M_PI / 180.0);
	orbitInfo_.keplerian.w = orbit_info.keplerian.w * (M_PI / 180.0);
	orbitInfo_.keplerian.W = orbit_info.keplerian.W * (M_PI / 180.0);
	orbitInfo_.keplerian.M0 = orbit_info.keplerian.M0 * (M_PI / 180.0);
}

int Planet:: setTargetTime (double julianday)
{
	targetJDay_ = julianday;

	return updateCartesian ();
}

void Planet:: getTargetTime (double *julianday) const
{
	*julianday = targetJDay_;
}

void Planet:: getPosition (double *position) const
{
	for (int i = 0; i < 3; i++) {
		position [i] = position_ (i);
	}
}

void Planet:: getVelocity (double *velocity) const
{
	for (int i = 0; i < 3; i++) {
		velocity [i] = velocity_ (i);
	}
}

void Planet:: getPosition (Vector3d *position) const
{
	*position = position_;
}

void Planet:: getVelocity (Vector3d *velocity) const
{
	*velocity = velocity_;
}

int Planet:: updateCartesian (void)
{
	double E;	// eccentric anomaly at the target time
	if (solveKeplerEquation (&E)) {
		return 1;
	}

	double a = orbitInfo_.keplerian.a;
	double e = orbitInfo_.keplerian.e;
	double i = orbitInfo_.keplerian.i;
	double w = orbitInfo_.keplerian.w;
	double W = orbitInfo_.keplerian.W;

	double n = sqrt (MueSun_ / (a*a*a));
	double dotE = n / (1.0 - e * cos (E));

	Vector3d posPerigee, velPerigee;	// from Sun to perigee: x-axis, orbit up: z-axis
	const int X = 0, Y = 1, Z = 2;

	posPerigee[X] = a * (cos (E) - e);
	posPerigee[Y] = a * sqrt (1.0 - e * e) * sin (E);
	posPerigee[Z] = 0.0;

	velPerigee[X] = -1.0 * a * dotE * sin (E);
	velPerigee[Y] = a * sqrt (1.0 - e * e) * dotE * cos (E);
	velPerigee[Z] = 0.0;
	
	Matrix3d C1;
	tf:: calcDcm (&C1, Z, -w);
	Matrix3d C2;
	tf:: calcDcm (&C2, X, -i);
	Matrix3d C3;
	tf:: calcDcm (&C3, Z, -W);

	// transform to the value in Sun Centerd Inertial coordinate
	position_ = C3 * C2 * C1 * posPerigee;
	velocity_ = C3 * C2 * C1 * velPerigee;
	
	return 0;
}

int Planet:: solveKeplerEquation (double *eccentric_anomaly)
{
	double a = orbitInfo_.keplerian.a;
	double e = orbitInfo_.keplerian.e;
	double M0 = orbitInfo_.keplerian.M0;
	double epoch = orbitInfo_.epochJDay;

	double n = sqrt (MueSun_ / (a*a*a));
	const double SecondsDay = 24.0 * 3600.0;
	double dt = (targetJDay_ - epoch) * SecondsDay;

	double M = M0 + n * dt;	// mean anomaly at target time

	double E;	// eccentric anomaly
	double tmpE = M;
	
	const int MaxIteration = 1000;
	const double MaxResidual = 1.0e-9;
	int iteration = 0;

	do {
		E = M + e * sin (tmpE);
		tmpE = E;
		
		if (iteration > MaxIteration) {
			return 1;
		}
		iteration++;

	} while (abs (E - tmpE) > MaxResidual);
	
	*eccentric_anomaly = E;

	return 0;
}