/*
 *  pm_BandpassFilter.cpp
 *  bassAmp
 *
 *  Created by Bill on 11/7/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "pm_BandpassFilter.h"


pm_BandpassFilter::pm_BandpassFilter(double alpha, double beta, double gamma)
			: pm_FilterBase(alpha, beta, gamma)
{
	
	alpha = alpha;
	beta = beta;
	gamma =  gamma;
}

pm_BandpassFilter::pm_BandpassFilter(pm_FilterDesign  fd)
			:pm_FilterBase(fd)
{
	fd = fd;
}

// Run the filter algorithm
float pm_BandpassFilter::doFilter(Float32 inBuffer, Float32 outBuffer,int length){
	float tBuffer;
	iIndex = 0; //temp for test - iIndex is not initialized anywhere
	
	for (int index=0; index < length; index++) {
		
		
		// Fetch sample - original
		inArray[iIndex] = inBuffer;
	
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
		
		//outBuffer[index] += amplitudeAdj * out;  //original
		
		tBuffer = inBuffer + amplitudeAdj * out;
		
		iIndex = (iIndex + 1) % HISTORYSIZE;
		 
	}
	return tBuffer ;
	
}