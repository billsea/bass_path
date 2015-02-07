/*
 *  pm_FilterDesignBase.h
 *  bassAmp
 *
 *  Created by Bill on 11/7/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

// Bandpass filter based on Java Audio example

#ifndef PM_FILTERDESIGNBASE_H
#define PM_FILTERDESIGNBASE_H

#include "math.h"

class pm_FilterDesignBase
{
public:
	
	pm_FilterDesignBase(int frequency, int sampleRate,double parameter);
	
	// Given a frequency of interest, calculate radians/sample
	double calcRadiansPerSample(double freq);
	
	// Return the radians per sample at the frequency of interest
	double getThetaZero();
	
	// Do the design of the filter. Filter response controlled by
	// just three coefficients: alpha, beta and gamma.
	void doFilterDesign();
	
	// Print all three IIR coefficients
	void printCoefficients();
	
	// Return alpha coefficient
	double getAlpha();
	
	// Return beta coefficient
	double getBeta();
	
	// Return gamma coefficient
	double getGamma();
	
	
//private:
	// Private class data
	int frequency;
	int sampleRate;
	double parameter;
	double alpha;
	double beta;
	double gamma;

};

#endif