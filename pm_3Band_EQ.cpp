/*
 *  pm_3Band_EQ.cpp
 *  bassAmp
 *
 *  Created by Bill on 11/6/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "pm_3Band_EQ.h"

// NOTES :
//
// - Original filter code by Paul Kellet (musicdsp.pdf)
//
// - Uses 4 first order filters in series, should give 24dB per octave
//
// - Now with P4 Denormal fix :)

#include <math.h>
#include "pm_3Band_EQ.h"


	
static double vsa = (1.0 / 4294967295.0);   // Very small amount (Denormal Fix)

pm_3Band_EQ::pm_3Band_EQ()
{


}

// ---------------
//| Initialise EQ |
// ---------------

// Recommended frequencies are ...
//
//  lowfreq  = 880  Hz
//  highfreq = 5000 Hz
//
// Set mixfreq to whatever rate your system is using (eg 48Khz)

void pm_3Band_EQ::init_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq)
{
	// Clear state 
	sampleRate = mixfreq;
	
	memset(es,0,sizeof(EQSTATE));
	
	// Set Low/Mid/High gains to unity
	
	es->lg = 1.0;
	es->mg = 1.0;
	es->hg = 1.0;
	
	// Calculate filter cutoff frequencies
	
	es->lf = 2 * sin(M_PI * ((double)lowfreq / (double)mixfreq)); 
	es->hf = 2 * sin(M_PI * ((double)highfreq / (double)mixfreq));
}

void pm_3Band_EQ::setFreq(EQSTATE* es,int lowfreq, int highfreq)
{
	// Calculate filter cutoff frequencies
	
	es->lf = 2 * sin(M_PI * ((double)lowfreq / (double)sampleRate)); 
	es->hf = 2 * sin(M_PI * ((double)highfreq / (double)sampleRate));
	
}

// ---------------
//| EQ one sample |
// ---------------

// - sample can be any range you like :)
//
// Note that the output will depend on the gain settings for each band 
// (especially the bass) so may require clipping before output, but you 
// knew that anyway :)

double pm_3Band_EQ::do_3band(EQSTATE* es, double sample)
{
	// Locals

	double  l,m,h;      // Low / Mid / High - Sample Values
	
	// Filter #1 (lowpass)
	
	es->f1p0  += (es->lf * (sample   - es->f1p0)) + vsa;
	es->f1p1  += (es->lf * (es->f1p0 - es->f1p1));
	es->f1p2  += (es->lf * (es->f1p1 - es->f1p2));
	es->f1p3  += (es->lf * (es->f1p2 - es->f1p3));
	
	l          = es->f1p3;
	
	// Filter #2 (highpass)
	
	es->f2p0  += (es->hf * (sample   - es->f2p0)) + vsa;
	es->f2p1  += (es->hf * (es->f2p0 - es->f2p1));
	es->f2p2  += (es->hf * (es->f2p1 - es->f2p2));
	es->f2p3  += (es->hf * (es->f2p2 - es->f2p3));
	
	h          = es->sdm3 - es->f2p3;
	
	// Calculate midrange (signal - (low + high))
	
	m          = es->sdm3 - (h + l);
	
	// Scale, Combine and store
	
	l         *= es->lg;
	m         *= es->mg;
	h         *= es->hg;
	
	// Shuffle history buffer 
	
	es->sdm3   = es->sdm2;
	es->sdm2   = es->sdm1;
	es->sdm1   = sample;                
	
	// Return result
	
	return(l + m + h);
}
