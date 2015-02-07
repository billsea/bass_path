/*
 *  pm_FilterDesignBase.cpp
 *  bassAmp
 *
 *  Created by Bill on 11/7/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "pm_FilterDesignBase.h"

pm_FilterDesignBase::pm_FilterDesignBase(int frequency, int sampleRate,double parameter)
{
	// Save incoming
	frequency = frequency;
	sampleRate = sampleRate;
	
	// Damping factor for highpass and lowpass, q for bandpass
	parameter = parameter;
}

// Given a frequency of interest, calculate radians/sample
double pm_FilterDesignBase::calcRadiansPerSample(double freq) {
	
	return (2.0 * pi * freq) / sampleRate;
}

void printCoefficients() {
	/*
	System.out.println("Filter Specifications:");
	 System.out.println("\tSample Rate: " + sampleRate +
						", Frequency: " + frequency + 
						", d/q: " + parameter);
	 
	 System.out.println("\tAlpha: " + alpha);
	 System.out.println("\tBeta: " + beta);
	 System.out.println("\tGamma: " + gamma);
	 */
}

// Return the radians per sample at the frequency of interest
double pm_FilterDesignBase::getThetaZero() {
	
	return calcRadiansPerSample(frequency);
}

// Return alpha coefficient
double pm_FilterDesignBase::getAlpha() {
	
	return alpha;
}

// Return beta coefficient
double pm_FilterDesignBase::getBeta() {
	
	return beta;
}

// Return gamma coefficient
double pm_FilterDesignBase::getGamma() {
	
	return gamma;
}