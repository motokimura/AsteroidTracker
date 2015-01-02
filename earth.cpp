#include "earth.h"

Earth:: Earth (void): Planet ()
{
	Planet:: OrbitInfo info;
	// http://nssdc.gsfc.nasa.gov/planetary/factsheet/earthfact.html, January 2014
	info.epochJDay = 2451545.00000;
	info.keplerian.a = 1.00000011;
	info.keplerian.e = 0.01671022;
	info.keplerian.i = 0.00005;
    info.keplerian.W = -11.26064;
	info.keplerian.w = 102.94719 - info.keplerian.W;
	info.keplerian.M0 = 100.46435 - info.keplerian.W - info.keplerian.w;

	Planet:: setOrbitInfo (info);
}

Earth:: ~Earth (void)
{
	//
}

int Earth:: setTargetTime (double julian_day)
{
	Planet:: setTargetTime (julian_day);
	updateGst ();

	return 0;
}

void Earth:: getGst (double* gst) const
{
	*gst = gst_;
}

void Earth:: updateGst (void)
{
	const double Pi = M_PI;
	
	double targetJDay;
	Planet:: getTargetTime (&targetJDay);

	double truncatedJDay;
	tf:: convertJd2TJd (&truncatedJDay, targetJDay);

	gst_ = (0.671262 + 1.0027379094 * truncatedJDay) * 2.0 * Pi;	// http://ja.wikipedia.org/wiki/%E6%81%92%E6%98%9F%E6%99%82
	tf:: normalizeRadian (&gst_);
}