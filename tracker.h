#ifndef TRACKER_H_
#define	TRACKER_H_

#include "earth.h"
#include "tf.h"

#include <float.h>

#include <Eigen/Eigen>
#include <Eigen/Dense>
using namespace Eigen;

class SpaceObjectTracker
{
public:
	SpaceObjectTracker (void);
	virtual ~SpaceObjectTracker (void);

	void setObserverGeoCoord (double latitude, double longitude, double altitude);	// [deg], [deg], [m]
	
	virtual int setTargetUnixtime (double target);
	void setEarth (Earth const& earth);
	void setTargetPosition (double const (&position)[3]);	// [m]
	void setTargetVelocity (double const (&velocity)[3]);	// [m/s]
	void setTargetPosition (Vector3d const& position);		// [m], over load, in Eigen::Vector3d
	void setTargetVelocity (Vector3d const& velocity);		// [m/s], over load, in Eigen::Vector3d

	virtual void getDepartureTime (double *departure) const;

	// observer position is NOT required.
	void getTargetRange (double *range) const;	// [km]
	void getTargetSpeed (double *speed) const;	// [m/s]
	void getTargetCelestrialPosition (double *declination, double *right_ascension) const;	// [deg], [deg]
	void getTargetGeoCoord (double *latitude, double *longitude, double *altitude) const;	// [deg], [deg], [m]

	// observer positio is required.
	void getTargetObserverRange (double *range) const;	// [km]
	void getTargetObserverSpeed (double *speed) const;	// [m/s]
	void getTargetDirection (double *elevation, double *azimuth) const;	// [deg], [deg]
	void getTargetDopplerCoeff (double *down, double *up) const;		// received_freq = down * downlink_freq, sent_freq = up * uplink_freq

	// observer position is NOT required.
	void getTargetPositionEci (double *pos_eci) const;		// [m]
	void getTargetVelocityEci (double *vel_eci) const;		// [m/s]
	void getTargetPositionEcef (double *pos_ecef) const;	// [m]
	void getTargetVelocityEcef (double *vel_ecef) const;	// [m/s]

	// observer positio is required.
	void getTargetObserverPositionEnu (double *pos_enu) const;	// [m]
	void getTargetObserverVelocityEnu (double *vel_enu) const;	// [m/s]
	void getObserverPositionEci (double *pos_eci) const;		// [m]
	void getObserverVelocityEci (double *vel_eci) const;		// [m/s]

	// observer position is NOT required.
	void getTargetPositionEci (Vector3d *pos_eci) const;	// [m], over load, out Eigen:: Vector3d
	void getTargetVelocityEci (Vector3d *vel_eci) const;	// [m/s], over load, out Eigen:: Vector3d
	void getTargetPositionEcef (Vector3d *pos_ecef) const;	// [m], over load, out Eigen:: Vector3d
	void getTargetVelocityEcef (Vector3d *vel_ecef) const;	// [m/s], over load, out Eigen:: Vector3d

	// observer positio is required.
	void getTargetObserverPositionEnu (Vector3d *pos_enu) const;// [m], over load, out Eigen:: Vector3d
	void getTargetObserverVelocityEnu (Vector3d *vel_enu) const;// [m/s], over load, out Eigen:: Vector3d
	void getObserverPositionEci (Vector3d *pos_eci) const;		// [m], over load, out Eigen:: Vector3d
	void getObserverVelocityEci (Vector3d *vel_eci) const;		// [m], over load, out Eigen:: Vector3d

private:
	static const double EarthObliquity_;
	static const double EarthOblatness_;
	static const double EarthRadius_;
	static const double EarthSpinRate_;
	static const double LightSpeed_;

	typedef struct {
		Vector3d position;	// [m]
		Vector3d velocity;	// [m/s]
	} CartesianSci;
	
	typedef struct {
		double latitude;	// [rad]
		double longitude;	// [rad]
		double altitude;	// [m]
	} GeoCoord;

	Earth earth_;
	CartesianSci target_;
	GeoCoord observer_;

	void convertSciRelative2Eci (Vector3d *vec_eci, Vector3d const& relative_vec_sci) const;
	void convertEci2Ecef (Vector3d *vec_ecef, Vector3d const& vec_eci) const;
	void convertEciRelative2Enu (Vector3d *vec_enu, Vector3d const& relative_vec_eci) const;
	void calcTargetRaySpeed (double *speed, Vector3d const& relative_pos, Vector3d const& relative_vel) const;
};

#endif // !TRACKER_H_
