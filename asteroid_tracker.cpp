#include "asteroid_tracker.h"

AsteroidTracker:: AsteroidTracker (void): SpaceObjectTracker ()
{
	//
}

AsteroidTracker:: ~AsteroidTracker (void)
{
	//
}

void AsteroidTracker:: setAsteroidOrbitInfo (Planet::OrbitInfo const& orbit_info)
{
	asteroid_.setOrbitInfo (orbit_info);
}

void AsteroidTracker:: setEarthOrbitInfo (Planet::OrbitInfo const& orbit_info)
{
	earth_.setOrbitInfo (orbit_info);
}

int AsteroidTracker:: setTargetUnixtime (double unixtime)
{
	double target_julianday;
	tf::convertUnixtime2Jd (&target_julianday, unixtime);

	// update earth orbit
	if (earth_.setTargetTime (target_julianday)) {
		return 1;
	}
	SpaceObjectTracker:: setEarth (earth_);

	// update target orbit
	if (asteroid_.setTargetTime (target_julianday)) {
		return 2;
	}
	Vector3d ta_pos, ta_vel;
	asteroid_.getPosition (&ta_pos);
	asteroid_.getVelocity (&ta_vel);
	SpaceObjectTracker:: setTargetPosition (ta_pos);
	SpaceObjectTracker:: setTargetVelocity (ta_vel);

	return 0;
}

void AsteroidTracker:: getTargetTime (double *julianday) const
{
	asteroid_.getTargetTime (julianday);
}

void AsteroidTracker:: getTargetPositionSci (double* position) const
{
	asteroid_.getPosition (position);
}

void AsteroidTracker:: getTargetVelocitySci (double* velocity) const
{
	asteroid_.getVelocity (velocity);
}