#include "asteroid_despatch.h"
#include "pass.h"

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

int trackDespatch (AsteroidDespatchTracker *tracker, double begin, double end, double interval);
int searchPasses (AsteroidDespatchTracker *tracker, double begin, double end);
int testTrajectory (AsteroidDespatchTracker *tracker);

int main (void)
{
	// inputs
	double unixtime_begin = 1417587612.0;							// departure, 2014/12/03 06:20:12 UTC
	double unixtime_end = unixtime_begin + 30.0 * 24.0 * 3600.0;
	double interval = 24.0 * 3600.0;	// [sec]
	double observer_lat = 35.610603;	// [deg]
	double observer_lon = 139.351124;	// [deg]
	double observer_alt = 148.0;		// [m]

	AsteroidDespatchTracker tracker;	// tracker class instance, whose constructor has DESPATCH orbit elements and departure time
	tracker.setObserverGeoCoord (observer_lat, observer_lon, observer_alt);	// set the observer position
	
	trackDespatch (&tracker, unixtime_begin, unixtime_end, interval);	// get tracking data
	searchPasses (&tracker, unixtime_begin, unixtime_end);				// search passes
    //testTrajectory (&tracker);

	return 0;
}

int trackDespatch (AsteroidDespatchTracker *tracker, double begin, double end, double interval)
{
	double time = begin;
	cout << setprecision (15) << endl;
	cout << "unixtime,elevation[deg],azimuth[deg],doppler_down,doppler_up,distance[km],declination[deg],right_ascension[deg],phase" << endl;
	do 
	{
		// set the target time
		int ret = tracker->setTargetUnixtime (time);
		
		// check the returned value
		if (ret == 0) {
			// Succeeded in orbit calculation
		}
		else if (ret == 1) 
		{
			cout << "[ERROR] Failed to calculate Earth orbit." << endl;
			return ret;
		}
		else if (ret == 2) 
		{
			cout << "[ERROR] Failed to calculate Despatch orbit." << endl;
			return ret;
		}
		else if (ret == 3) 
		{
			cout << "[ERROR] Target time is out of the range." << endl;
			double departure_unixtime;
			tracker->getDepartureTime (&departure_unixtime);
			cout << "[ERROR] Target time must be no less than departure time: " << departure_unixtime << "[UNIXTIME]" << endl;
			return ret;
		}

		// outputs
		double elevation, azimuth;			// [deg]
		double doppler_down, doppler_up;	// [-]
		double distance;					// [km]
		double declination, right_ascension;// [deg]
		string phase;						// "1", "2", "3", "radio_stop", or "not_defined"

		// get valuse and output
		tracker->getTargetDirection (&elevation, &azimuth);
		tracker->getTargetDopplerCoeff (&doppler_down, &doppler_up);
		tracker->getTargetObserverRange (&distance);
		tracker->getTargetCelestrialPosition (&declination, &right_ascension);
		tracker->getDespatchPhase (&phase);

		cout << time << ",";
		cout << elevation << "," << azimuth << ",";
		cout << doppler_down << "," << doppler_up << ",";
		cout << distance << ",";
		cout << declination << "," << right_ascension << ",";
		cout << phase << endl;

		time += interval;

	} while (time < end);

	return 0;
}

int searchPasses (AsteroidDespatchTracker *tracker, double begin, double end)
{
	PassFinder finder;
    
	vector<PassFinder:: Pass> passes = finder.findAll(tracker, begin, end, 60.0);

	cout << setprecision (10) << endl;
	cout << "AOS, LOS, TCA" << endl;
	for (vector<PassFinder:: Pass>::iterator it = passes.begin (); it != passes.end (); ++it) {
		cout << it->AOS << "," << it->LOS << "," << it->TCA << endl;
	}
	cout << endl;

	return 0;
}

int testTrajectory (AsteroidDespatchTracker *tracker)
{
    const double AU = 149597871000.0;   // meter
    
    double begin = 1420070400.0;
    double end = begin + 400.0 * 24.0 * 3600.0;
    double interval = 24.0 * 3600.0;
    
    double time = begin;
    cout << setprecision (12);
    cout << "unixtime, x, y, z" << endl;
    
    double pos_sc[3];
    
    do 
    {
            tracker->setTargetUnixtime (time);
            tracker->getTargetPositionSci (pos_sc);
            
            cout << time << ",";
            cout << pos_sc[0] / AU << "," << pos_sc[1] / AU << "," << pos_sc[2] / AU << "," << endl;
            
            time += interval;
            
    } while (time < end);
    
    return 0;
}