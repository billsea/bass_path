/*
 *  pm_FilterDesign.h
 *  bassAmp
 *
 *  Created by Bill on 11/7/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef PM_FILTERDESIGN_H
#define PM_FILTERDESIGN_H

#include "pm_FilterDesignBase.h"

class pm_FilterDesign : public pm_FilterDesignBase
{
public:
	
	pm_FilterDesign(int centerFrequency, int sampleRate, double q);
	
	// Do the design of the filter. Filter response controlled by
	// just three coefficients: alpha, beta and gamma.
	void doFilterDesign();
	
	/*
	// Entry point for testing
	static void main(string args[]) {
		
		if (args.length != 3) {
			//System.out.println("\nIIR Bandpass Filter Design Program");
			//System.out.println("\nUsage:");
			//System.out.println("\tIIRBandpassFilterDesign " +
			//				   "centerFrequency sampleRate q");
			
			//System.exit(1);
		}
	
		// Parse the command line arguments
	int centerFrequency;//	= Integer(args[0]).intValue();
	int sampleRate;//		=  Integer(args[1]).intValue();
		double q 	;//		=  Double(args[2]).doubleValue();
	
		
		// Instantiate bandpass filter designer
		  pm_FilterDesign d(centerFrequency, sampleRate, q);
		 d.centerFrequency = centerFrequency;
		 d.sampleRate=sampleRate;
		 d.q=q;
	
		// Do the design
		d.doFilterDesign();
	
		// Print out the coefficients
		//d.printCoefficients();
*/
	
};

#endif