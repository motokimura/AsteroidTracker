#ifndef PLANET_H_
#define PLANET_H_

#include "tf.h"

#include "eigen-eigen-6b38706d90a9/eigen/Eigen"
#include "eigen-eigen-6b38706d90a9/eigen/Dense"
using namespace Eigen;

class Planet 
{
public:
	typedef struct {
		double a;		// semi-major axis, [AU]
		double e;		// eccentricity
		double i;		// inclination, [deg]
		double w;		// argument of perihelion, [deg]
		double W;		// longitude of the ascending node, [deg]
		double M0;		// mean anomaly at epoch, [deg]
	} Keplerian;

	typedef struct {
		Keplerian keplerian;
		double epochJDay;	// [Julian day]
	} OrbitInfo;

	Planet (void);
	virtual ~Planet (void);

	void setOrbitInfo (OrbitInfo const& orbit_info);

	virtual int setTargetTime (double julianday);
	void getTargetTime (double *julianday) const;

	void getPosition (double *position) const;		// [m]
	void getVelocity (double *velocity) const;		// [m/s]

	void getPosition (Vector3d *position) const;	// [m]
	void getVelocity (Vector3d *velocity) const;	// [m/s]

private:
	static const double MueSun_;// [m^3 kg^-1 s^-2]
	static const double AU_;	// [m]

	OrbitInfo orbitInfo_;	// units are converted, AU->meter, rad->deg 

	double targetJDay_;	// [Julian day]
	Vector3d position_;	// [m]
	Vector3d velocity_;	// [m/s]

	int updateCartesian (void);
	int solveKeplerEquation (double *eccentric_anomaly);	// [rad]
};

#endif // !PLANET_H_
