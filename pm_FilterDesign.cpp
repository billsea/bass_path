/*
 *  pm_FilterDesign.cpp
 *  bassAmp
 *
 *  Created by Bill on 11/7/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "pm_FilterDesign.h"


pm_FilterDesign::pm_FilterDesign(int alpha, int beta, double gamma)
: pm_FilterDesignBase(alpha, beta, gamma)
{

	alpha = alpha;
	beta = beta;
	gamma =  gamma;
	
}

void pm_FilterDesign::doFilterDesign() {
	
	// thetaZero = 2 * Pi * Freq * T or (2 * Pi * Freq) / sampleRate
	// where Freq is center frequency of bandpass filter
	double thetaZero = getThetaZero();
	
	double theTan = tan(thetaZero / (2.0 * parameter));
	
	// Beta relates gain to bandwidth (and therefore q) at -3 db points
	beta = 0.5 * ((1.0 - theTan) / (1.0 + theTan));
	
	// For unity gain at center frequency
	alpha = (0.5 - beta) / 2.0;
	
	// Final filter coefficient
	gamma = (0.5 + beta) * cos(thetaZero);
}