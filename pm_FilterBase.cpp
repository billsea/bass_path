/*
 *  pm_FilterBase.cpp
 *  bassAmp
 *
 *  Created by Bill on 11/7/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "pm_FilterBase.h"

// pm_FilterBase class constructor
// alpha, beta and gamma are precalculated filter coefficients
// that are passed into this filter element.
pm_FilterBase::pm_FilterBase(double alpha, double beta, double gamma) {
	
	// Save incoming
	alpha = alpha;
	beta = beta;
	gamma = gamma;
	
	amplitudeAdj = 1.0;
	
	// Allocate history buffers
	//inArray  = double[HISTORYSIZE];
	//outArray = double[HISTORYSIZE];
}


pm_FilterBase::pm_FilterBase(pm_FilterDesignBase fdb) {
	fdb.getAlpha();
	fdb.getBeta();
	fdb.getGamma();
}

void pm_FilterBase::updateFilterCoefficients(pm_FilterDesignBase fdb) {
	
	alpha = fdb.getAlpha();
	beta  = fdb.getBeta();
	gamma = fdb.getGamma();
}

void pm_FilterBase::setAlpha(double alpha) {
	
	alpha = alpha;
}

void pm_FilterBase::setBeta(double beta) {
	
	beta = beta;
}

void pm_FilterBase::setGamma(double gamma) {
	
	gamma = gamma;
}

// Abstract method that runs the filter algorithm
void pm_FilterBase::doFilter(Float32 inBuffer, Float32 outBuffer,int length)
{
	
	for (int index=0; index < length; index++) {
		
		// Fetch sample
		inArray[iIndex] = inBuffer;//[index];
		
		// Do indices maintainance
		jIndex = iIndex - 2;
		if (jIndex < 0) jIndex += HISTORYSIZE;
		kIndex = iIndex - 1;
		if (kIndex < 0) kIndex += HISTORYSIZE;
		
		// Run the difference equation
		double out = outArray[iIndex] = 
			2 * (alpha * (inArray[iIndex] - inArray[jIndex]) + 
				 gamma * outArray[kIndex] -
				 beta  * outArray[jIndex]);
		
		outBuffer/*[index]*/ += amplitudeAdj * out;
		
		iIndex = (iIndex + 1) % HISTORYSIZE;
	}
}
// Set the amplitude adjustment to be applied to filtered data
// Values typically range from -.25 to +4.0 or -12 to +12 db.
void pm_FilterBase::setAmplitudeAdj(double amplitudeAdj) {
	
	amplitudeAdj = amplitudeAdj;
}