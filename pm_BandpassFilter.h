/*
 *  pm_BandpassFilter.h
 *  bassAmp
 *
 *  Created by Bill on 11/7/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef PM_BANDPASSFILTER_H
#define PM_BANDPASSFILTER_H


#include "pm_FilterDesign.h"
#include "pm_FilterBase.h"

// Optimized IIR bandpass filter with only 4 multiplies per sample
// Used for each band of the graphic equalizer.
	
class pm_BandpassFilter : public pm_FilterBase
{
public:
	
	
	// IIRBandpassFilter class constructor
	// alpha, beta and gamma are precalculated filter coefficients
	// that are passed into this filter element.
	pm_BandpassFilter(double alpha, double beta, double gamma);
	
	// Filter coefficients can also be extracted by passing in 
	// design object.
	
	pm_BandpassFilter(pm_FilterDesign  fd);
	
	// Run the filter algorithm
	float doFilter(Float32 inBuffer, Float32 outBuffer,int length) ;
	
	//~ pm_BandpassFilter(); //destructor
	
};

#endif