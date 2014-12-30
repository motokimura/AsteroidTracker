#include "tf.h"

void tf:: calcDcm (Matrix3d* ans, int axis, double rad)
{
	const int X = 0, Y = 1, Z = 2;

	switch (axis) {
		
		case X:
			*ans <<
				1.0,        0.0,       0.0,
				0.0,  cos (rad), sin (rad),
				0.0, -sin (rad), cos (rad);
			break;
		
		case Y:
			*ans <<
				cos (rad), 0.0, -sin (rad),
				      0.0, 1.0,        0.0,
				sin (rad), 0.0,  cos (rad);
			break;
			
		case Z:
			*ans << 
				 cos (rad), sin (rad), 0.0,
				-sin (rad), cos (rad), 0.0,
				       0.0,       0.0, 1.0;
			break;
		
		default:
			*ans = Matrix3d:: Identity ();
			break;
	}
}

void tf:: normalizeRadian (double* radian, double start_radian)
{
	const double TwoPi = 2.0 * M_PI;
	
	double rad = *radian;
	
	if (rad < 0.0 || rad > TwoPi) {
		
		int div = static_cast<int> ( (rad - start_radian) / TwoPi);
		rad -= static_cast<double> (div) * TwoPi;
		
		if (rad < start_radian) {
			rad += TwoPi;
		}
	}
	
	*radian = rad;
}

void tf:: diffRadians (double *ans, double rad1, double rad2)
{
	const double Pi = M_PI;
	
	double diff = rad1 - rad2;
	normalizeRadian (&diff);
	
	if (diff > Pi) {
		diff -= 2.0 * Pi;
	}
	
	*ans = diff;
}

void tf:: convertMJd2Jd (double* julianday, double modified_julianday)
{
	const double JDayOffsetFromMJDay = 2400000.5;
	*julianday = modified_julianday + JDayOffsetFromMJDay;
}

void tf:: convertJd2TJd (double* truncated_julianday, double julianday)
{
	const double TJDayOffsetFromJDay = 2440000.5;
	*truncated_julianday = julianday - TJDayOffsetFromJDay;
}

void tf:: convertMJd2Unixtime (double* unixtime, double modified_julianday)
{
	const double MJDayAtUnixtimeOrigin = 40587.0;
	const double SecondsDay = 24.0 * 3600.0;
	
	*unixtime = (modified_julianday - MJDayAtUnixtimeOrigin) * SecondsDay;
}

void tf:: convertUnixtime2Jd (double* julianday, double unixtime)
{
	const double SecondsDay = 24.0 * 3600.0;
	const double JDayAtUnixtimeOrigin = 2440587.5;
	
	*julianday = JDayAtUnixtimeOrigin + unixtime / SecondsDay;
}

void tf:: convertJd2Unixtime (double* unixtime, double julianday)
{
	const double SecondsDay = 24.0 * 3600.0;
	const double JDayAtUnixtimeOrigin = 2440587.5;

	*unixtime = (julianday - JDayAtUnixtimeOrigin) * SecondsDay;
}