/*
 *  pmOverdrive.h
 *  bassAmp
 *
 *  Created by Bill on 12/13/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PMOVERDRIVE
#define PMOVERDRIVE
#include <string>
#include "pmHiPassFilter.h"
#include "TwelveAX7.h"
#include "basics.h"
#include "util.h"

using namespace primoHPFilter;
using namespace DSP;

class pmOverdrive
{
public:
	pmOverdrive(int sampRate);
	virtual ~pmOverdrive() {}
	
	//void Crush(Float32 &in1, float freq, float bits);
	void Distort(Float32 &in1, float amount);
	void Overdrive(Float32 &in1, float amount);
	void setSampleRate(float sampRate);
	
	//virtual void Reset() {}
protected:
	int m_SampleRate;
	float mLastCutoff_F1;
	float mLastResonance_F1;
	pm_HiPassFilter hpFilter;
	TwelveAX7 tube;
	
	Float32 hp_filter(Float32 input, Float32 trebCor, Float32 cutBoost);
};



#endif
