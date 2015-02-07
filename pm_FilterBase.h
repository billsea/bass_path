/*
 *  pm_FilterBase.h
 *  bassAmp
 *
 *  Created by Bill on 11/7/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef PM_FILTERBASE_H
#define PM_FILTERBASE_H

#include "pm_FilterDesignBase.h"

// Base class for all IIR filters.

class pm_FilterBase
{
public:
	
	const static int HISTORYSIZE = 3;
	
	// IIRFilterBase class constructor
	// alpha, beta and gamma are precalculated filter coefficients
	// that are passed into this filter element.
	pm_FilterBase(double alpha, double beta, double gamma);
	
	// Filter coefficients can also be extracted by passing in 
	// design object.
	pm_FilterBase(pm_FilterDesignBase fdb);
	
	 void updateFilterCoefficients(pm_FilterDesignBase fdb);
	
	 void setAlpha(double alpha);
	 void setBeta(double beta);
	
	 void setGamma(double gamma);
	
	// Abstract method that runs the filter algorithm
	void doFilter(Float32 inBuffer, Float32 outBuffer,int length);
	
	// Set the amplitude adjustment to be applied to filtered data
	// Values typically range from -.25 to +4.0 or -12 to +12 db.
	void setAmplitudeAdj(double amplitudeAdj);
	
	// Private class data
//private:
	double alpha;
	double beta;
	double gamma;
	double amplitudeAdj;
	Float32 inArray[HISTORYSIZE];
	Float32 outArray[HISTORYSIZE];
	int iIndex;
	int jIndex;
	int kIndex;
};

#endif