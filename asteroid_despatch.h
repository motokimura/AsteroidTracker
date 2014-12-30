#ifndef ASTEROID_DESPATCH_H_
#define	ASTEROID_DESPATCH_H_

#include "planet.h"
#include "asteroid_tracker.h"
#include "tf.h"

#include <string>

class AsteroidDespatchTracker: public AsteroidTracker
{
public:
	AsteroidDespatchTracker (void);
	~AsteroidDespatchTracker (void);

	int setTargetUnixtime (double unixtime);

	void getDepartureTime (double *unixtime) const;
	void getDespatchDownlinkFrequency (double *MHz) const;	// [MHz]
	void getDespatchPhase (std:: string *phase) const;		// string "1", "2", "3", "radio_stop", or "not_defined"

private:
	static const double DepartureJulianday_;
	static const double DespatchTransmitterFrequency_;	// [MHz]
	static const double SecondsMorseEnd_;	// seconds from the departure
	static const double SecondsBaudotEnd_;	// seconds from the departure
	static const double SecondsBeaconEnd_;	// seconds from the departure

	void calcSecondsFromDeparture (double *second) const;
};

#endif // !ASTEROID_DESPATCH_H_
