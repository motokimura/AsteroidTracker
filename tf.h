#ifndef TF_H_
#define TF_H_

#include <Eigen/Eigen>
#include <Eigen/Dense>
using namespace Eigen;

namespace tf {
	void calcDcm (Matrix3d* ans, int axis, double rad);
	
	void normalizeRadian (double* radian, double start_radian = 0.0);
	void diffRadians (double* ans, double rad1, double rad2);
	
	void convertMJd2Jd (double* julianday, double modified_julianday);
	void convertJd2TJd (double* truncated_julianday, double julianday);
	void convertMJd2Unixtime (double* unixtime, double modified_julianday);
	void convertUnixtime2Jd (double* julianday, double unixtime);
	void convertJd2Unixtime (double* unixtime, double julianday);
};

#endif	// !TF_H_