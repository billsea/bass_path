/*
 *  pmOverdrive.cpp
 *  bassAmp
 *
 *  Created by Bill on 12/13/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "pmOverdrive.h"

using namespace primoHPFilter;
using namespace DSP;

pmOverdrive::pmOverdrive(int sampRate) { m_SampleRate = sampRate; }

void pmOverdrive::setSampleRate(float sampRate) { m_SampleRate = sampRate; }

void pmOverdrive::Distort(Float32 &in1, float amount) {
  if (amount > 0) {
    float tbtrans = tube.transfer(in1);
    float inHPF = tube.transfer_clip(
        hp_filter(tbtrans, 100, -(amount * 5))); // add HP filter

    if (amount >= 0.99)
      amount = 0.99;

    float k = 8 * amount / (1 - amount); // float k=2*amount/(1-amount);
                                         // original

    in1 = ((1 + k) * inHPF / (1 + k * fabs(inHPF))) * (1 - amount);
  }
}

void pmOverdrive::Overdrive(Float32 &in1, float amount) {
  if (amount > 0) {
    float tbtrans = tube.transfer(in1);
    float inHPF = tube.transfer_clip(
        hp_filter(tbtrans, 100, -(amount * 5))); // add HP filter

    if (amount >= 0.99)
      amount = 0.99;
    float k = 8 * amount / (1 - amount);

    in1 = (((1 + k) * inHPF / (1 + k * fabs(inHPF))) * (1 - amount)) +
          (in1 * 0.05);
    ; // add unprocessed signal
  }
}

// highpass filter
Float32 pmOverdrive::hp_filter(Float32 input, Float32 trebCor,
                               Float32 cutBoost) {
  float resonance = cutBoost;
  float srate = m_SampleRate;
  Float32 cutoff = trebCor;

  cutoff = 0.5 * cutoff / srate;
  if (cutoff > 0.99)
    cutoff = 0.99;

  if (cutoff != mLastCutoff_F1 || resonance != mLastResonance_F1) {
    hpFilter.CalculateHipassParams(cutoff, resonance);
    mLastCutoff_F1 = cutoff;
    mLastResonance_F1 = resonance;
  }

  // process Highpass filter
  float H = hpFilter.processInput(input);

  return H;
}
