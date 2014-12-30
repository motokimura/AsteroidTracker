#ifndef PASS_H_
#define	PASS_H_

#include "tracker.h"
#include <vector>

class PassFinder {
public:
	typedef struct
	{
		double AOS, LOS, TCA;
	} Pass;

	PassFinder (void);
	~PassFinder (void);

	Pass findFirst (SpaceObjectTracker* tracker, double begin_unix, double end_unix, double interval) const;
	std:: vector<Pass> findAll (SpaceObjectTracker* tracker, double begin_unix, double end_unix, double interval) const;

private:

};

#endif // !PASS_H_
