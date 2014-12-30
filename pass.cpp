#include "pass.h"

PassFinder:: PassFinder (void)
{
	//
}

PassFinder:: ~PassFinder (void)
{
	//
}

PassFinder:: Pass PassFinder:: findFirst (SpaceObjectTracker* tracker, double begin_unix, double end_unix, double interval) const
{
	static const Pass error_pass = {-1., -1., -1.};
	
	if (!tracker) {
		return error_pass;
	}

	double departure_unix;
	tracker->getDepartureTime (&departure_unix);
	if (begin_unix < departure_unix) {
		begin_unix = departure_unix;
	}

	if (begin_unix > end_unix) {
		return error_pass;
	}

	// Search AOS
	Pass pass = {begin_unix, begin_unix, begin_unix};
	
	if (tracker->setTargetUnixtime (pass.AOS) != 0) {
		return error_pass;
	}

	double elevation,azimuth;
	tracker->getTargetDirection (&elevation, &azimuth);

	if (elevation > 0.0) {
		while (elevation > 0.0) {
			pass.AOS -= interval;
			
			if (pass.AOS < departure_unix) {
				pass.AOS = departure_unix;
				break;
			}

			if (tracker->setTargetUnixtime (pass.AOS) != 0) {
				return error_pass;
			}
			tracker->getTargetDirection (&elevation, &azimuth);
		}
	}

	while (elevation < 0.0) {
		pass.AOS += interval;

		if (pass.AOS > end_unix) {
			return error_pass;
		}

		if (tracker->setTargetUnixtime (pass.AOS) != 0) {
			return error_pass;
		}
		tracker->getTargetDirection (&elevation, &azimuth);
	}
	
	// Search LOS and TCA
	pass.LOS = pass.AOS;
	double max_elevation = 0.0;
	
	while (elevation > 0.0) {
		pass.LOS += interval;
		
		if (tracker->setTargetUnixtime (pass.LOS) != 0) {
			return error_pass;
		}
		tracker->getTargetDirection (&elevation, &azimuth);
		
		if (max_elevation < elevation) {
			max_elevation = elevation;
			pass.TCA = pass.LOS;
		}
	}

	return pass;
}

std:: vector<PassFinder:: Pass> PassFinder:: findAll(SpaceObjectTracker* tracker, double begin_unix, double end_unix, double interval) const
{
	std:: vector<Pass> passes;
	Pass pass = findFirst(tracker, begin_unix, end_unix, interval);
	
	while (pass.AOS > 0) {
		passes.push_back (pass);
		pass = findFirst (tracker, pass.LOS, end_unix, interval);
	}
	
	return passes;
}