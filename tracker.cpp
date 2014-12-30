#include "tracker.h"

const double SpaceObjectTracker:: EarthObliquity_ = -23.44 * M_PI / 180.0;
const double SpaceObjectTracker:: EarthOblatness_ = 1.0 / 298.257223563;
const double SpaceObjectTracker:: EarthRadius_ = 6.378137e6f;
const double SpaceObjectTracker:: EarthSpinRate_ = 2.0 * M_PI / 23.9345 / 3600.0;
const double SpaceObjectTracker:: LightSpeed_ = 299792458.0;

SpaceObjectTracker:: SpaceObjectTracker (void)
{
	//
}

SpaceObjectTracker:: ~SpaceObjectTracker (void)
{
	//
}

void SpaceObjectTracker:: setObserverGeoCoord (double latitude, double longitude, double altitude)
{
	observer_.latitude = latitude * M_PI / 180.0;
	observer_.longitude = longitude * M_PI / 180.0;
	observer_.altitude = altitude;
}

int SpaceObjectTracker:: setTargetUnixtime (double target)
{
	return -1;
}

void SpaceObjectTracker:: setEarth (Earth const& earth)
{
	earth_ = earth;
}

void SpaceObjectTracker:: setTargetPosition (double const (&position)[3])
{
	for (int i = 0; i < 3; i++) {
		target_.position[i] = position[i];
	}
}

void SpaceObjectTracker:: setTargetVelocity (double const (&velocity)[3])
{
	for (int i = 0; i < 3; i++) {
		target_.velocity[i] = velocity[i];
	}
}

void SpaceObjectTracker:: setTargetPosition (Vector3d const& position)
{
	target_.position = position;
}

void SpaceObjectTracker:: setTargetVelocity (Vector3d const& velocity)
{
	target_.velocity = velocity;
}

void SpaceObjectTracker:: getDepartureTime (double *departure) const
{
	*departure = -DBL_MAX;
}

void SpaceObjectTracker:: getTargetRange (double *range) const
{
	Vector3d pos_eci;
	getTargetPositionEci (&pos_eci);

	*range = pos_eci.norm () / 1000.0;
}

void SpaceObjectTracker:: getTargetSpeed (double *speed) const
{
	Vector3d pos_eci, vel_eci;
	getTargetPositionEci (&pos_eci);
	getTargetVelocityEci (&vel_eci);

	calcTargetRaySpeed (speed, pos_eci, vel_eci);
}

void SpaceObjectTracker:: getTargetCelestrialPosition (double *declination, double *right_ascension) const
{
	Vector3d pos_eci;
	getTargetPositionEci (&pos_eci);

	const int X  = 0, Y = 1, Z = 2;

	*declination = atan2 (pos_eci[Z], sqrt (pos_eci[X] * pos_eci[X] + pos_eci[Y] * pos_eci[Y]));
	*right_ascension = atan2 (pos_eci[Y], pos_eci[X]);

	tf:: normalizeRadian (declination, -M_PI);
	tf:: normalizeRadian (right_ascension);

	*declination *= 180.0 / M_PI;
	*right_ascension *= 180.0 / M_PI;
}

void SpaceObjectTracker:: getTargetGeoCoord (double *latitude, double *longitude, double *altitude) const
{
	Vector3d pos_ecef;
	getTargetPositionEcef (&pos_ecef);
	
	double a = EarthRadius_;
	double b = a * (1.0 - EarthOblatness_);
	double e = sqrt (2.0 * EarthOblatness_ - EarthOblatness_ * EarthOblatness_);
	double n = a * a - b * b;
	
	const int X = 0, Y = 1, Z = 2;
	double p = sqrt (pos_ecef[X] * pos_ecef[X] + pos_ecef[Y] * pos_ecef[Y]);
	double t = atan2 (pos_ecef[Z] * a, p * b);
	double sint = sin (t);
	double cost = cos (t);
	
	double lat = atan2 (pos_ecef[Z] + n/b * sint*sint*sint, p - n/a *cost*cost*cost);
	double h = a / sqrt (1.0 - e * e * sin (lat) * sin (lat));
	*longitude = atan2 (pos_ecef[Y], pos_ecef[X]);
	*altitude = (p / cos (lat)) - h;
	*latitude = lat;
	
	tf:: normalizeRadian (latitude, -M_PI);
	tf:: normalizeRadian (longitude, -M_PI);

	*latitude *= 180.0 / M_PI;
	*longitude *= 180.0 / M_PI;
}

void SpaceObjectTracker:: getTargetObserverRange (double *range) const
{
	Vector3d ta_pos;
	getTargetPositionEci (&ta_pos);

	Vector3d obs_pos;
	getObserverPositionEci (&obs_pos);

	Vector3d relative_pos = ta_pos - obs_pos;

	*range = relative_pos.norm () / 1000.0;
}

void SpaceObjectTracker:: getTargetObserverSpeed (double *speed) const
{
	Vector3d ta_pos, obs_pos, relative_pos;
	getTargetPositionEci (&ta_pos);
	getObserverPositionEci (&obs_pos);
	relative_pos = ta_pos - obs_pos;

	Vector3d ta_vel, obs_vel, relative_vel;
	getTargetVelocityEci (&ta_vel);
	getObserverVelocityEci (&obs_vel);
	relative_vel = ta_vel - obs_vel;

	calcTargetRaySpeed (speed, relative_pos, relative_vel);
}

void SpaceObjectTracker:: getTargetDirection (double *elevation, double *azimuth) const
{
	Vector3d pos_enu;
	getTargetObserverPositionEnu (&pos_enu);
	
    const int X  = 0, Y = 1, Z = 2;
	*elevation = atan2 (pos_enu[Z], sqrt (pos_enu[X] * pos_enu[X] + pos_enu[Y] * pos_enu[Y]));
	*azimuth = atan2 (pos_enu[X], pos_enu[Y]);
	
	tf:: normalizeRadian (elevation, -M_PI);
	tf:: normalizeRadian (azimuth);

	*elevation *= 180.0 / M_PI;
	*azimuth *= 180.0 / M_PI;
}

void SpaceObjectTracker:: getTargetDopplerCoeff (double *down, double *up) const
{
	double speed;
	getTargetObserverSpeed (&speed);
    double ratio = - speed / LightSpeed_;

	*down = 1.0 + ratio;
    *up = 1.0 - ratio;
}


void SpaceObjectTracker:: getTargetPositionEci (double *pos_eci) const
{
	Vector3d vec;
	getTargetPositionEci (&vec);

	for (int i = 0; i < 3; i++) {
		pos_eci[i] = vec[i];
	}
}

void SpaceObjectTracker:: getTargetVelocityEci (double *vel_eci) const
{
	Vector3d vec;
	getTargetVelocityEci (&vec);

	for (int i = 0; i < 3; i++) {
		vel_eci[i] = vec[i];
	}
}

void SpaceObjectTracker:: getTargetPositionEcef (double *pos_ecef) const
{
	Vector3d vec;
	getTargetPositionEcef (&vec);

	for (int i = 0; i < 3; i++) {
		pos_ecef[i] = vec[i];
	}
}

void SpaceObjectTracker:: getTargetVelocityEcef (double *vel_ecef) const
{
	Vector3d vec;
	getTargetVelocityEcef (&vec);

	for (int i = 0; i < 3; i++) {
		vel_ecef[i] = vec[i];
	}
}

void SpaceObjectTracker:: getTargetObserverPositionEnu (double *pos_enu) const
{
	Vector3d vec;
	getTargetObserverPositionEnu (&vec);

	for (int i = 0; i < 3; i++) {
		pos_enu[i] = vec[i];
	}
}

void SpaceObjectTracker:: getTargetObserverVelocityEnu (double *vel_enu) const
{
	Vector3d vec;
	getTargetObserverVelocityEnu (&vec);

	for (int i = 0; i < 3; i++) {
		vel_enu[i] = vec[i];
	}
}

void SpaceObjectTracker:: getObserverPositionEci (double *pos_eci) const
{
	Vector3d vec;
	getObserverPositionEci (&vec);

	for (int i = 0; i < 3; i++) {
		pos_eci[i] = vec[i];
	}
}

void SpaceObjectTracker:: getObserverVelocityEci (double *vel_eci) const
{
	Vector3d vec;
	getObserverVelocityEci (&vec);

	for (int i = 0; i < 3; i++) {
		vel_eci[i] = vec[i];
	}
}

void SpaceObjectTracker:: getTargetPositionEci (Vector3d *pos_eci) const
{
	Vector3d earth_pos;
	earth_.getPosition (&earth_pos);

	Vector3d relative_pos = target_.position - earth_pos;

	convertSciRelative2Eci (pos_eci, relative_pos);
}

void SpaceObjectTracker:: getTargetVelocityEci (Vector3d *vel_eci) const
{
	Vector3d earth_vel;
	earth_.getVelocity (&earth_vel);

	Vector3d relative_vel = target_.velocity - earth_vel;

	convertSciRelative2Eci (vel_eci, relative_vel);
}

void SpaceObjectTracker:: getTargetPositionEcef (Vector3d *pos_ecef) const
{
	Vector3d pos_eci;
	getTargetPositionEci (&pos_eci);

	convertEci2Ecef (pos_ecef, pos_eci);
}

void SpaceObjectTracker:: getTargetVelocityEcef (Vector3d *vel_ecef) const
{
	Vector3d vel_eci;
	getTargetVelocityEci (&vel_eci);

	convertEci2Ecef (vel_ecef, vel_eci);
}

void SpaceObjectTracker:: getTargetObserverPositionEnu (Vector3d *pos_enu) const
{
	Vector3d ta_pos_eci;
	getTargetPositionEci (&ta_pos_eci);
	
	Vector3d obs_pos_eci;
	getObserverPositionEci (&obs_pos_eci);

	Vector3d rel_pos_eci = ta_pos_eci - obs_pos_eci;
	
	convertEciRelative2Enu (pos_enu, rel_pos_eci);
}

void SpaceObjectTracker:: getTargetObserverVelocityEnu (Vector3d *vel_enu) const
{
	Vector3d ta_vel_eci;
	getTargetVelocityEci (&ta_vel_eci);
	
	Vector3d obs_vel_eci;
	getObserverVelocityEci (&obs_vel_eci);

	Vector3d rel_vel_eci = ta_vel_eci - obs_vel_eci;
	
	convertEciRelative2Enu (vel_enu, rel_vel_eci);
}

void SpaceObjectTracker:: getObserverPositionEci (Vector3d *pos_eci) const
{
	double lat = observer_.latitude;
	double lon = observer_.longitude;
	double alt = observer_.altitude;
	
	double e = sqrt (2.0 * EarthOblatness_ - EarthOblatness_ * EarthOblatness_);
	double h = EarthRadius_ / sqrt (1.0 - e * e * sin (lat) * sin (lat));
	
	double gst;
	earth_.getGst (&gst);
	
	const int X = 0, Y = 1, Z = 2;
	Vector3d obs_pos;
	
	// http://www.enri.go.jp/~fks442/K_MUSEN/1st/1st021118.pdf
	obs_pos[X] = (h + alt) * cos (lat) * cos (lon + gst);
	obs_pos[Y] = (h + alt) * cos (lat) * sin (lon + gst);
	obs_pos[Z] = (h * (1.0 - e * e) + alt) * sin (lat);
	
	*pos_eci = obs_pos;
}

void SpaceObjectTracker:: getObserverVelocityEci (Vector3d *vel_eci) const
{
	double lat = observer_.latitude;
	double lon = observer_.longitude;
	double alt = observer_.altitude;
	
	double e = sqrt (2.0 * EarthOblatness_ - EarthOblatness_ * EarthOblatness_);
	double h = EarthRadius_ / sqrt (1.0 - e * e * sin (lat) * sin (lat));
	
	double gst;
	earth_.getGst (&gst);
	
	const int X = 0, Y = 1, Z = 2;
	Vector3d obs_vel;
	
	// http://www.enri.go.jp/~fks442/K_MUSEN/1st/1st021118.pdf
	obs_vel[X] = -(h + alt) * EarthSpinRate_ * cos (lat) * sin (lon + gst);
	obs_vel[Y] =  (h + alt) * EarthSpinRate_ * cos (lat) * cos (lon + gst);
	obs_vel[Z] =  0.0;
	
	*vel_eci = obs_vel;
}

void SpaceObjectTracker:: convertSciRelative2Eci (Vector3d *vec_eci, Vector3d const& relative_vec_sci) const
{
	const int X = 0;
	Matrix3d C;
	tf:: calcDcm (&C, X, EarthObliquity_);

	*vec_eci = C * relative_vec_sci;
}

void SpaceObjectTracker:: convertEci2Ecef (Vector3d *vec_ecef, Vector3d const& vec_eci) const
{
	double gst;
	earth_.getGst (&gst);
	
	const int Z = 2;
	Matrix3d C;
	tf:: calcDcm (&C, Z, gst);
	
	*vec_ecef = C * vec_eci;
}

void SpaceObjectTracker:: convertEciRelative2Enu (Vector3d *vec_enu, Vector3d const& relative_vec_eci) const
{
	double gst;
	earth_.getGst (&gst);

	double lon = observer_.longitude;
	double lat = observer_.latitude;

	const int Y = 1, Z = 2;
	Matrix3d C1, C2, C3;
	tf:: calcDcm (&C1, Z, gst + lon);
	tf:: calcDcm (&C2, Y, M_PI / 2.0 - lat);
	tf:: calcDcm (&C3, Z, M_PI / 2.0);
	
	*vec_enu = C3 * C2 * C1 * relative_vec_eci;
}

void SpaceObjectTracker:: calcTargetRaySpeed (double *speed, Vector3d const& relative_pos, Vector3d const& relative_vel) const
{
	*speed = relative_vel.dot (relative_pos.normalized ());
}

