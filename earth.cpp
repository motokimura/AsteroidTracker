#include "earth.h"

Earth:: Earth (void): Planet ()
{
	Planet:: OrbitInfo info;
	// Rika-nenpyo H.26
	info.epochJDay = 2456800.5;
	info.keplerian.a = 1.0;
	info.keplerian.e = 0.0167;
	info.keplerian.i = 0.002;
	info.keplerian.w = 102.985;
	info.keplerian.W = 354.838;
	info.keplerian.M0 = 137.350;

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