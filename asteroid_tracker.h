#ifndef ASTEROID_TRACKER_H_
#define ASTEROID_TRACKER_H_

#include "planet.h"
#include "tracker.h"
#include "tf.h"

#include <Eigen/Eigen>
#include <Eigen/Dense>
using namespace Eigen;

class AsteroidTracker: virtual public SpaceObjectTracker
{
public:
	AsteroidTracker (void);
	virtual ~AsteroidTracker (void);

	void setAsteroidOrbitInfo (Planet::OrbitInfo const& orbit_info);
	void setEarthOrbitInfo (Planet::OrbitInfo const& orbit_info);
	virtual int setTargetUnixtime (double unixtime);
	void getTargetTime (double *julianday) const;

	void getTargetPositionSci (double* position) const;	// [m]
	void getTargetVelocitySci (double* velocity) const;	// [m/s]

private:
	Planet asteroid_;
	Earth earth_;
};

#endif // !ASTEROID_TRACKER_H_
