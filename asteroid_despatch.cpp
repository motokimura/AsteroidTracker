#include "asteroid_despatch.h"

const double AsteroidDespatchTracker:: DepartureJulianday_ = 2456994.764023;	// 2014/12/03 06:20:12 UTC
const double AsteroidDespatchTracker:: DespatchTransmitterFrequency_ = 437.325;
const double AsteroidDespatchTracker:: SecondsMorseEnd_ = 9.0 * 3600.0;
const double AsteroidDespatchTracker:: SecondsBaudotEnd_ = 108.0 * 3600.0;
const double AsteroidDespatchTracker:: SecondsBeaconEnd_ = (30.0 * 24.0) * 3600.0;

AsteroidDespatchTracker:: AsteroidDespatchTracker (void): AsteroidTracker ()
{
	Planet:: OrbitInfo despatch_orbit;
	
	despatch_orbit.epochJDay = 2457023.5;		// 2015/1/01 00:00:00 UTC
	despatch_orbit.keplerian.a = 1.021898798;	// [AU]
	despatch_orbit.keplerian.e = 0.09221357958;	// [-]
	despatch_orbit.keplerian.i = 6.740416311;	// [deg]
	despatch_orbit.keplerian.w = 107.4801793;	// [deg]
	despatch_orbit.keplerian.W = 245.4163230;	// [deg]
	despatch_orbit.keplerian.M0 = 90.28021171;	// [deg]

	AsteroidTracker:: setAsteroidOrbitInfo (despatch_orbit);
}

AsteroidDespatchTracker:: ~AsteroidDespatchTracker (void)
{
	//
}

int AsteroidDespatchTracker:: setTargetUnixtime (double unixtime)
{
	double target_julianday;
	tf::convertUnixtime2Jd (&target_julianday, unixtime);

	if (target_julianday < DepartureJulianday_)
	{
		return 3;
	}

	return AsteroidTracker:: setTargetUnixtime (unixtime);
}

void AsteroidDespatchTracker:: getDepartureTime (double *unixtime) const
{
	double departure_unixtime;
	tf::convertJd2Unixtime (&departure_unixtime, DepartureJulianday_);

	*unixtime = departure_unixtime;
}

void AsteroidDespatchTracker:: getDespatchDownlinkFrequency (double *MHz) const
{
	double down, up;
	SpaceObjectTracker:: getTargetDopplerCoeff (&down, &up);

	*MHz = down * DespatchTransmitterFrequency_;
}

void AsteroidDespatchTracker:: getDespatchPhase (std:: string *phase) const
{
	double seconds_from_departure;
	calcSecondsFromDeparture (&seconds_from_departure);
	
	if (seconds_from_departure > SecondsBeaconEnd_) {
		*phase = "radio_stop";
	}
	else if (seconds_from_departure > SecondsBaudotEnd_) {
		*phase = "3";
	}
	else if (seconds_from_departure > SecondsMorseEnd_) {
		*phase = "2";
	}
	else if (seconds_from_departure > 0.0) {
		*phase = "1";
	}
	else {
		*phase = "not_defined";
	}
}

void AsteroidDespatchTracker:: calcSecondsFromDeparture (double* second) const
{
	const double SecondsDay = 24.0 * 3600.0;

	double target_julianday;
	AsteroidTracker::getTargetTime (&target_julianday);

	*second = (target_julianday - DepartureJulianday_) * SecondsDay;
}