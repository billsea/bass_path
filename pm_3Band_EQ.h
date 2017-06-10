/*
 *  pm_3Band_EQ.h
 *  bassAmp
 *
 *  Created by Bill on 11/6/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 How to use ...

 1. First you need to declare a state for your eq

 EQSTATE eq;

 2. Now initialise the state (we'll assume your output frequency is 48Khz)

 set_3band_state(eq,880,5000,480000);

 Your EQ bands are now as follows (approximatley!)

 low band = 0Hz to 880Hz
 mid band = 880Hz to 5000Hz
 high band = 5000Hz to 24000Hz

 3. Set the gains to some values ...

 eq.lg = 1.5; // Boost bass by 50%
 eq.mg = 0.75; // Cut mid by 25%
 eq.hg = 1.0; // Leave high band alone

 4. You can now EQ some samples

 out_sample = do_3band(eq,in_sample)
 */

#ifndef __pm_3Band_EQ__
#define __pm_3Band_EQ__


// ------------
//| Structures |
// ------------

class pm_3Band_EQ
{
public:
	pm_3Band_EQ();
	
typedef struct 
{
	// Filter #1 (Low band)
	
	double  lf;       // Frequency
	double  f1p0;     // Poles ...
	double  f1p1;     
	double  f1p2;
	double  f1p3;
	
	// Filter #2 (High band)
	
	double  hf;       // Frequency
	double  f2p0;     // Poles ...
	double  f2p1;
	double  f2p2;
	double  f2p3;
	
	// Sample history buffer
	
	double  sdm1;     // Sample data minus 1
	double  sdm2;     //                   2
	double  sdm3;     //                   3
	
	// Gain Controls
	
	double  lg;       // low  gain
	double  mg;       // mid  gain
	double  hg;       // high gain
	
} EQSTATE;  

int sampleRate;

// ---------
//| Exports |
// ---------

void init_3band_state(EQSTATE* es, int lowfreq, int highfreq, int mixfreq);
double do_3band(EQSTATE* es, double sample);
void setFreq(EQSTATE* es,int lowfreq, int highfreq);


private:



};

#endif // #ifndef __pm_3Band_EQ__
