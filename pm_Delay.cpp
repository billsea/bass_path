/*
 *  pm_Delay.cpp
 *  bassAmp
 *
 *  Created by Bill on 11/3/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
#include "AUEffectBase.h"
#include "pm_Delay.h"
#include "math.h"

namespace primoMedia_Delay
{
	


pm_Delay::pm_Delay()
{
	const float pkMaxDelayTime =5.0;
	pmMaxDelayFrames = int(sampRate * pkMaxDelayTime + 10);
	delayBuffer_L.AllocateClear(pmMaxDelayFrames);
	delayBuffer_R.AllocateClear(pmMaxDelayFrames);
	pmWriteIndex = pmMaxDelayFrames - 1;
	
}


////////////////////////////
void pm_Delay::processInput(Float32 &srcInL, Float32 &srcInR)
{
	//single parameter values
	float	level;
	int		readIndex;
	float phaseGain = 0.2; 
	
	int frames1 = delayTime  * sampRate; //delay frames
	
	if (frames1 < 1) frames1 = 1;
	else if (frames1 >= pmMaxDelayFrames) frames1 = pmMaxDelayFrames - 1;
	
	
	level = phaseGain;
	readIndex = pNormalizeIndex(pmWriteIndex - frames1);	// read head lags write head...
	
	// intensity
	float wetDry = -1 * ((wdLevel / 100.) - 1);	// percent to ratio - inverted
	float wet = sqrt(wetDry), dry = sqrt(1.0 - wetDry);
	
	// original for loop start//////////
	
	//write to audio buffer index
	delayBuffer_L[pmWriteIndex] = srcInL;
	delayBuffer_R[pmWriteIndex] = srcInR;
	pmWriteIndex = (pmWriteIndex + 1) % pmMaxDelayFrames;
	
	Float32 outputL = dry * srcInL ;
	Float32 outputR = dry * srcInR ;
	
	int readIndex1 = readIndex;
	Float32 delayReadL = delayBuffer_L[readIndex1];
	Float32 delayReadR = delayBuffer_R[readIndex1];
	
	readIndex = (readIndex1 + 1) % pmMaxDelayFrames;
	
	outputL += delayReadL * level * wet ;
	outputR += delayReadR * level * wet ;
	
	srcInL = outputL; 
	srcInR = outputR; 
	
}
////////////////////////////
void pm_Delay::setWriteIndex( int wIndex )
{
	pmWriteIndex = wIndex;
}

int pm_Delay::getWriteIndex()
{
	
	return pmWriteIndex;
}

int		pm_Delay::pNormalizeIndex(int i) 
{
	return (i + pmMaxDelayFrames) % pmMaxDelayFrames;
}


void   pm_Delay::setSampleRate( double sampleRate )
{
	 sampRate = sampleRate;
	
}

void pm_Delay::setDelayTime(Float32 delTime)
{
	delayTime = delTime;
}
void pm_Delay::setWetDryLevel(Float32 level)
{
	wdLevel = level;
}


}
