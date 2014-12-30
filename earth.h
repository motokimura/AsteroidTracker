#ifndef EARTH_H_
#define EARTH_H_

#include "planet.h"
#include "tf.h"

#include "eigen-eigen-6b38706d90a9/eigen/Eigen"
#include "eigen-eigen-6b38706d90a9/eigen/Dense"
using namespace Eigen;

class Earth: public Planet
{
public:
	Earth (void);
	~Earth (void);

	int setTargetTime (double julian_day);
	void getGst (double* gst) const;	// [rad]

private:
	double gst_;	// [rad]
	void updateGst (void);
};

#endif // !EARTH_H_
