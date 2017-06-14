/*
 *  pm_Delay.h
 *  bassAmp
 *
 *  Created by Bill on 11/3/06.
 *  Copyright 2006 primo-media.com. All rights reserved.
 *
 */

#include "AUEffectBase.h"
#include "math.h"
//#include <AudioToolbox/AudioUnitUtilities.h>

namespace primoMedia_Delay {

class pm_Delay {

public:
  pm_Delay();

  void setDelayTime(Float32 delTime);
  void setWetDryLevel(Float32 level);
  void processInput(Float32 &srcInL, Float32 &srcInR);
  void setSampleRate(double sampleRate);
  void setWriteIndex(int wIndex);
  int getWriteIndex();

  // properties
private:
  int pNormalizeIndex(int i);

  TAUBuffer<Float32> delayBuffer_L;
  TAUBuffer<Float32> delayBuffer_R;

  Float32 delayTime;
  Float32 wdLevel;

  int pmWriteIndex;
  int pmMaxDelayFrames;

  double sampRate;
};
}
