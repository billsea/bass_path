/*
*	File:		bassAmp.cpp
*	
*	Version:	1.0
* 
*	Created:	9/19/06
*	
*	Copyright:  Copyright © 2006 primo-media.com, All Rights Reserved
* 

*
*/

#include "bassAmp.h"
#include "AUEffectBase.h"
#include "bassAmpVersion.h"
#include "math.h"
#include <AudioToolbox/AudioUnitUtilities.h>
#include "SimpleLimit.h"
#include "LFO.h"
#include "pm_FilterDesign.h"
#include "pm_3Band_EQ.h"
#include "pmOverdrive.h"
#include "Cabinet.h"

float CurrentSampleRate=44100;//just to initialize
pmOverdrive dirt(CurrentSampleRate);


//three band eq L
pm_3Band_EQ tbEQL;
pm_3Band_EQ::EQSTATE eq3L;

//three band eq R
pm_3Band_EQ tbEQR;
pm_3Band_EQ::EQSTATE eq3R;

//cabinet simulator
CabinetII cab;

//LFO for chorus effect
LFO chorusLFO(CurrentSampleRate);

using namespace chunkware_simple;
SimpleLimit limiter;

// parameters
static const float kDefaultValue_ParamOne_Indexed=0;
static const float kDefaultValue_ParamTwo=0.7;
static const float kDefaultValue_ParamThree=0.0;
static const float kDefaultValue_ParamFour=0.0;
static const float kDefaultValue_ParamFive=0;
static const float kDefaultValue_ParamSix=800;
static const float kDefaultValue_ParamSeven=4500;
static const float kDefaultValue_ParamEight=0.0;
static const float kDefaultValue_ParamNine=0.0;
static const float kDefaultValue_ParamTen=0.0;
static const float kDefaultValue_ParamEleven=0.020;
static const float kDefaultValue_ParamTwelve=0.05;
static const float kDefaultValue_ParamThirteen=100.0;
static const float kDefaultValue_ParamFourteen= 0.25;
static const float kDefaultValue_ParamFifteen=100.0;
static const float kDefaultValue_ParamSixteen_Indexed=0;

static CFStringRef kParameterOne_IndexedName = CFSTR("Device");
static CFStringRef kParameterTwoName = CFSTR("Gain");
static CFStringRef kParameterThreeName = CFSTR("Dirt");
static CFStringRef kParameterFourName = CFSTR("Limiter");
static CFStringRef kParameterFiveName = CFSTR("Cabinet");
static CFStringRef kParameterSixName = CFSTR("EQ Low");
static CFStringRef kParameterSevenName = CFSTR("EQ High");
static CFStringRef kParameterEightName = CFSTR("Cut/Boost Low");
static CFStringRef kParameterNineName = CFSTR("Cut/Boost Mid");
static CFStringRef kParameterTenName = CFSTR("Cut/Boost High");
static CFStringRef kParameterElevenName = CFSTR("Chorus");
static CFStringRef kParameterTwelveName = CFSTR("Rate");
static CFStringRef kParameterThirteenName = CFSTR("Wet/Dry");
static CFStringRef kParameterFourteenName = CFSTR("Delay");
static CFStringRef kParameterFifteenName = CFSTR("Wet/Dry");
static CFStringRef kParameterSixteen_IndexedName = CFSTR("Cabinet Type");
static CFStringRef kParameterSeventeenName = CFSTR("Gain Reduction");
//cabinet type

static CFStringRef kParameterValueStringsOne_cab= CFSTR( "1" );
static CFStringRef kParameterValueStringsTwo_cab= CFSTR( "2" );
static CFStringRef kParameterValueStringsThree_cab= CFSTR( "3" );
static CFStringRef kParameterValueStringsFour_cab= CFSTR( "4" );
static CFStringRef kParameterValueStringsFive_cab= CFSTR( "5" );
static CFStringRef kParameterValueStringsSix_cab= CFSTR( "6" );

//index names for parameter one
static CFStringRef kParameterValueStringsOne   = CFSTR( "Bypass" );
static CFStringRef kParameterValueStringsTwo   = CFSTR( "On" );



enum {
	kParam_One_Indexed,
	kParam_Two,
	kParam_Three,
	kParam_Four,
	kParam_Five,
	kParam_Six,
	kParam_Seven,
	kParam_Eight,
	kParam_Nine,
	kParam_Ten,
	kParam_Eleven,
	kParam_Twelve,
	kParam_Thirteen,
	kParam_Fourteen,
	kParam_Fifteen,
	kParam_Sixteen_Indexed,
	kParam_Seventeen,
	kNumberOfParameters=25
};


//delay
TAUBuffer<Float32>	mDelayBufferL;
TAUBuffer<Float32>	mDelayBufferR;
int					mWriteIndex;
int					mMaxDelayFrames;
const float kMaxDelayTime = 5.0;	// seconds

//chorus
TAUBuffer<Float32>	mChorusBufferL;
TAUBuffer<Float32>	mChorusBufferR;
long				mWriteIndex_Chorus;
long				mMaxDelayFrames_Chorus;
const float kMaxDelayTime_Chorus = 5.0;	// seconds


// Factory presets
static const int kPreset_One = 0;
static const int kPreset_Two = 1;
static const int kPreset_Three = 2;
static const int kPreset_Four = 3;
static const int kNumberPresets = 4;

static AUPreset kPresets[kNumberPresets] = 
{
	{ kPreset_One, CFSTR("Dirty Metal") },		
	{ kPreset_Two, CFSTR("Squeaky Fusion") },
	{ kPreset_Three, CFSTR("Long Live Rock") },
	{ kPreset_Four, CFSTR("Slapper Boy") }
};



#pragma mark ____bassAmp
class bassAmp : public AUEffectBase
{
public:
	bassAmp(AudioUnit component);
	

	
#if AU_DEBUG_DISPATCHER
	virtual ~bassAmp () { delete mDebugDispatcher; }
#endif
	
	
	//virtual AUKernelBase *		NewKernel() { return new bassAmpKernel(this); }
	
	virtual	ComponentResult		GetParameterValueStrings(AudioUnitScope			inScope,
														 AudioUnitParameterID		inParameterID,
														 CFArrayRef *			outStrings);
    
	virtual	ComponentResult		GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo);
    
	virtual ComponentResult		GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &			outDataSize,
												Boolean	&			outWritable );
	
	virtual ComponentResult		GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 		inElement,
											void *			outData);
	
 	virtual	bool				SupportsTail () { return false; }
	
	/*! @method Version */
	virtual ComponentResult	Version() { return kbassAmpVersion; }
	
	
	//overrides process method
	virtual OSStatus            ProcessBufferLists(
												   AudioUnitRenderActionFlags &	ioActionFlags,
												   const AudioBufferList &			inBuffer,
												   AudioBufferList &				outBuffer,
												   UInt32							inFramesToProcess );
	
	// handle presets:
    virtual ComponentResult		GetPresets(	CFArrayRef	*outData	)	const;    
	virtual OSStatus			NewFactoryPresetSet (	const AUPreset & inNewFactoryPreset	);
	
	
protected:
		
		void Reset();
	int NormalizeIndex(int i) ;
	
	void bassAmp::fb_Distortion(Float32 &inL, Float32 &inR, Float32 threshold);
	void bassAmp::pmDelay(Float32 &inL, Float32 &inR, Float32 delayTime, Float32 wetDryLevel);
	void bassAmp::pmChorus(Float32 &inL, Float32 &inR, Float32 ChDelayTime, float chRate, Float32 wetDryLevel);
	void bassAmp::pmCabinet(Float32 &inL, Float32 &inR, Float32 gain, Float32 wetDryLevel);
	double bassAmp::dB2linear(double dbValue);

	
	class bassAmpKernel : public AUKernelBase		// most of the real work happens here
	{
		
		
public:
		
	
		
		//overridden by ProcessBufferLists
		/*
		// *Required* overides for the process method for this effect
		// processes one channel of interleaved samples
        virtual void 		Process(	const Float32 	*inSourceP,
										Float32		 	*inDestP,
										UInt32 			inFramesToProcess,
										UInt32			inNumChannels,
										bool			&ioSilence);
		
		*/
private: 
		
	};
	
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(bassAmp)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	bassAmp::bassAmp
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bassAmp::bassAmp(AudioUnit component)
: AUEffectBase(component)
{
	
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	
	// 3 band EQ 
	tbEQL.init_3band_state(&eq3L,880,5000,44100);
	tbEQR.init_3band_state(&eq3R,880,5000,44100);
	//limiter
	limiter.setSampleRate(GetSampleRate());
	
	//delay settings
	mMaxDelayFrames = (int)(GetSampleRate() * kMaxDelayTime + 10);
	
	mDelayBufferL.AllocateClear(mMaxDelayFrames);
	mDelayBufferR.AllocateClear(mMaxDelayFrames);
	mWriteIndex = mMaxDelayFrames - 1;
	
	//chorus settings
	mMaxDelayFrames_Chorus = (int)(GetSampleRate() * kMaxDelayTime_Chorus + 10);
	//a little extra so we can support full delay time
	
	mChorusBufferL.AllocateClear(mMaxDelayFrames);
	mChorusBufferR.AllocateClear(mMaxDelayFrames);
	mWriteIndex_Chorus = mMaxDelayFrames_Chorus - 1;
	
	// distortion set
	dirt.setSampleRate(GetSampleRate());
	
	//cabinet
	cab.init(GetSampleRate());
	cab.modelNum=2;//5 is marshall
	cab.activate();

	SetParameter(kParam_One_Indexed, kDefaultValue_ParamOne_Indexed ); 
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
	SetParameter(kParam_Five, kDefaultValue_ParamFive );
	SetParameter(kParam_Six, kDefaultValue_ParamSix );
	SetParameter(kParam_Seven, kDefaultValue_ParamSeven);
	SetParameter(kParam_Eight, kDefaultValue_ParamEight);
	SetParameter(kParam_Nine, kDefaultValue_ParamNine );
	SetParameter(kParam_Ten, kDefaultValue_ParamTen );
	SetParameter(kParam_Eleven, kDefaultValue_ParamEleven );
	SetParameter(kParam_Twelve, kDefaultValue_ParamTwelve );
	SetParameter(kParam_Thirteen, kDefaultValue_ParamThirteen );
	SetParameter(kParam_Fourteen, kDefaultValue_ParamFourteen );
	SetParameter(kParam_Fifteen, kDefaultValue_ParamFifteen );
	SetParameter(kParam_Sixteen_Indexed, kDefaultValue_ParamSixteen_Indexed );
	//SetParameter(kParam_Seventeen,0 );
		
	SetAFactoryPresetAsCurrent (kPresets[0]);
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
	

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	bassAmp::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		bassAmp::GetParameterValueStrings(AudioUnitScope		inScope,
														 AudioUnitParameterID	inParameterID,
														 CFArrayRef *		outStrings)
{
	
   	if (inScope != kAudioUnitScope_Global)
        return kAudioUnitErr_InvalidProperty;
    
    switch(inParameterID)
	{
        case kParam_One_Indexed:
            if(outStrings != NULL)
			{
                CFStringRef	strings[] =
			{
				kParameterValueStringsOne,
				kParameterValueStringsTwo,
			};
                *outStrings = CFArrayCreate( NULL, (const void **)strings, 2, NULL);
			}
			
            return noErr;
			
		case kParam_Sixteen_Indexed:
            if(outStrings != NULL)
			{
                CFStringRef	strings[] =
			{
				kParameterValueStringsOne_cab,
				kParameterValueStringsTwo_cab,
				kParameterValueStringsThree_cab,
				kParameterValueStringsFour_cab,
				kParameterValueStringsFive_cab,
				kParameterValueStringsSix_cab,
			};
                *outStrings = CFArrayCreate( NULL, (const void **)strings, 6, NULL);
			}
			
            return noErr;
			
			
            
        default:
            return kAudioUnitErr_InvalidProperty;
	}
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	bassAmp::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		bassAmp::GetParameterInfo(AudioUnitScope		inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;
	
	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
		|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
			case kParam_One_Indexed:
                AUBase::FillInParameterName (outParameterInfo, kParameterOne_IndexedName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 1;
                outParameterInfo.defaultValue = 0;
                outParameterInfo.flags |= kAudioUnitParameterFlag_NonRealTime;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
          		outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.5;
				outParameterInfo.defaultValue = 0.7;
                break;
				
			case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
				outParameterInfo.defaultValue = 0.0;
				outParameterInfo.flags += kAudioUnitParameterFlag_IsHighResolution;
                break;
			case kParam_Four:
                AUBase::FillInParameterName (outParameterInfo, kParameterFourName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
				outParameterInfo.minValue = -20.0;
				outParameterInfo.maxValue = 10.0;
				outParameterInfo.defaultValue = 0;
				//outParameterInfo.flags += kAudioUnitParameterFlag_IsHighResolution;
                break;
			case kParam_Five:
                AUBase::FillInParameterName (outParameterInfo, kParameterFiveName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Percent;
				outParameterInfo.minValue = 0.0;
				outParameterInfo.maxValue = 100.0;
				outParameterInfo.defaultValue = 50.0;
	
				outParameterInfo.flags += kAudioUnitParameterFlag_IsHighResolution;
                break;
			case kParam_Six:
                AUBase::FillInParameterName (outParameterInfo, kParameterSixName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Hertz;
				outParameterInfo.minValue = 20;
				outParameterInfo.maxValue = 8000;
				outParameterInfo.defaultValue = 200;
				//outParameterInfo.flags += kAudioUnitParameterFlag_DisplayLogarithmic;
                break;

			case kParam_Seven:
                AUBase::FillInParameterName (outParameterInfo, kParameterSevenName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Hertz;
				outParameterInfo.minValue = 400;
				outParameterInfo.maxValue = 10000;
				outParameterInfo.defaultValue = 4500;
				//outParameterInfo.flags += kAudioUnitParameterFlag_IsHighResolution;
                break;
				
			case kParam_Eight:
                AUBase::FillInParameterName (outParameterInfo, kParameterEightName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
				outParameterInfo.minValue = -20.0;
				outParameterInfo.maxValue = 20.0;
				outParameterInfo.defaultValue = 0.0;
				outParameterInfo.flags += kAudioUnitParameterFlag_IsHighResolution;
                break;
		
			case kParam_Nine:
                AUBase::FillInParameterName (outParameterInfo, kParameterNineName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
				outParameterInfo.minValue = -20.0;
				outParameterInfo.maxValue = 20.0;
				outParameterInfo.defaultValue = 0.0;
				outParameterInfo.flags += kAudioUnitParameterFlag_IsHighResolution;
                break;
	
			case kParam_Ten:
                AUBase::FillInParameterName (outParameterInfo, kParameterTenName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
				outParameterInfo.minValue = -20.0;
				outParameterInfo.maxValue = 20.0;
				outParameterInfo.defaultValue = 0.0;
				outParameterInfo.flags += kAudioUnitParameterFlag_IsHighResolution;
                break;
		
			case kParam_Eleven://chorus
				
                AUBase::FillInParameterName (outParameterInfo, kParameterElevenName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Seconds;
				outParameterInfo.minValue = 0.010;
				outParameterInfo.maxValue = 0.030;
				outParameterInfo.defaultValue = 0.025;
                break;
		
					
			case kParam_Twelve:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwelveName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Seconds;
				outParameterInfo.minValue = 0.001;
				outParameterInfo.maxValue = 0.50;
                outParameterInfo.defaultValue = 0.1;
				//outParameterInfo.flags += kAudioUnitParameterFlag_IsHighResolution;
                break;
			case kParam_Thirteen:
                AUBase::FillInParameterName (outParameterInfo, kParameterThirteenName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Percent;
				outParameterInfo.minValue = 0.0;
				outParameterInfo.maxValue = 100.0;
				outParameterInfo.defaultValue = 100.0;
                break;
			case kParam_Fourteen://delay
                AUBase::FillInParameterName (outParameterInfo, kParameterFourteenName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Seconds;
				outParameterInfo.minValue = 0.10;
				outParameterInfo.maxValue = 5.0;
				outParameterInfo.defaultValue = 0.54;
                break;
			case kParam_Fifteen:
                AUBase::FillInParameterName (outParameterInfo, kParameterFifteenName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Percent;
				outParameterInfo.minValue = 0.0;
				outParameterInfo.maxValue = 100.0;
				outParameterInfo.defaultValue = 100.0;
                break;
				
			case kParam_Sixteen_Indexed://cabinet type select
                AUBase::FillInParameterName (outParameterInfo, kParameterSixteen_IndexedName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 5;
                outParameterInfo.defaultValue = 2;
                outParameterInfo.flags |= kAudioUnitParameterFlag_NonRealTime;
                break;
			
			case kParam_Seventeen:
                AUBase::FillInParameterName (outParameterInfo, kParameterSeventeenName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 20.0;
				outParameterInfo.defaultValue = 0.0;
				//outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayMask;
                break;
		
            default:
                result = kAudioUnitErr_InvalidParameter;
                break;
		}
	} else {
        result = kAudioUnitErr_InvalidParameter;
    }
    
	
	
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	bassAmp::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		bassAmp::GetPropertyInfo (AudioUnitPropertyID	inID,
												 AudioUnitScope		inScope,
												 AudioUnitElement	inElement,
												 UInt32 &		outDataSize,
												 Boolean &		outWritable)
{
	if (inScope == kAudioUnitScope_Global) {
		switch (inID) {
			case kAudioUnitProperty_CocoaUI:
				outWritable = false;
				outDataSize = sizeof (AudioUnitCocoaViewInfo);
				return noErr;
				
		}
	}
	
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	bassAmp::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		bassAmp::GetProperty(	AudioUnitPropertyID inID,
												AudioUnitScope 		inScope,
												AudioUnitElement 	inElement,
												void *			outData )
{
	if (inScope == kAudioUnitScope_Global) {
		switch (inID) {
			case kAudioUnitProperty_CocoaUI:
			{
				// Look for a resource in the main bundle by name and type.
				CFBundleRef bundle = CFBundleGetBundleWithIdentifier( CFSTR("com.primomedia.audiounit.bassAmp") );
				
				if (bundle == NULL) return fnfErr;
                
				CFURLRef bundleURL = CFBundleCopyResourceURL( bundle, 
															  CFSTR("CocoaViewFactory"), 
															  CFSTR("bundle"), 
															  NULL);
                
                if (bundleURL == NULL) return fnfErr;
                
				CFStringRef className = CFSTR("bassAmp_CocoaViewFactory");
				AudioUnitCocoaViewInfo cocoaInfo = { bundleURL, className };
				*((AudioUnitCocoaViewInfo *)outData) = cocoaInfo;
				
				return noErr;
			}
		}
	}
	
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}


#pragma mark ____bassAmpEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	bassAmp::bassAmpKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		bassAmp::Reset()
{

}

int		bassAmp::NormalizeIndex(int i) 
{
	return (i + mMaxDelayFrames) % mMaxDelayFrames;
}

///////////////
//process buffer
////////////////

OSStatus bassAmp::ProcessBufferLists(
										AudioUnitRenderActionFlags &ioActionFlags,
										const AudioBufferList &inBufferList,
										AudioBufferList &outBufferList,
										UInt32 inFramesToProcess )
{
    // silent = ioActionFlags & kAudioUnitRenderAction_OutputIsSilence;
	
    const AudioBuffer *srcBuffer = inBufferList.mBuffers;
    AudioBuffer *destBuffer = outBufferList.mBuffers;
	
	// source 
	const Float32 *src1 = (const Float32 *) srcBuffer[0].mData;
    const Float32 *src2 = (const Float32 *) srcBuffer[1 % inBufferList.mNumberBuffers].mData;
	
	
	// destination
    Float32 *dst1 = (Float32 *) destBuffer[0].mData;
    Float32 *dst2 = (Float32 *) destBuffer[1 % outBufferList.mNumberBuffers].mData;

	//bypass
	int deviceSetting = GetParameter(kParam_One_Indexed );
	int cabinetSetting = GetParameter(kParam_Sixteen_Indexed );
	
	// Get Parameter settings
	// input gain
	Float32 inputGain = GetParameter( kParam_Two );
	// distortion
	Float32 dist = GetParameter( kParam_Three);
	// Limiter parameters
	Float32 thresh = GetParameter( kParam_Four );
	Float32 cabinetMix = GetParameter( kParam_Five);
	
	//Chorus parameters
	Float32 chorusDelay =  GetParameter( kParam_Eleven);
	Float32 chorusRate = GetParameter( kParam_Twelve);
	Float32 chorusLevel = GetParameter( kParam_Thirteen);
	
	// Delay parameters
	Float32 delayTime = GetParameter( kParam_Fourteen);
	Float32 delayLevel = GetParameter( kParam_Fifteen);
	
	Float32 filter1_freq = GetParameter(kParam_Six);
	Float32 filter2_freq = GetParameter(kParam_Seven);
	Float32 filter1_level = GetParameter(kParam_Eight);
	Float32 filter2_level = GetParameter(kParam_Nine);
	Float32 filter3_level = GetParameter(kParam_Ten);
	
	CurrentSampleRate = GetSampleRate();
	

	
	UInt32 frame;
	   for (frame = 0; frame < inFramesToProcess; frame++, src1++, src2++, dst1++, dst2++)
	   {
		  
		   
			if (deviceSetting==0){
				
				Float32 L_Chan = *src1;
				Float32 R_Chan= *src2;
				
				//process signal here
				
				///////////////////////////////////////////////////////////////////////////
				// Gain
				///////////////////////////////////////////////////////////////////////////
				
				L_Chan = inputGain * L_Chan;
				R_Chan = inputGain * R_Chan;
					
				
				///////////////////////////////////////////////////////////////////////////
				//distortion
				///////////////////////////////////////////////////////////////////////////

				dirt.Distort(L_Chan, dist);
				dirt.Distort(R_Chan, dist);
				
				// overdrive function adds clean signal to distorted
				//dirt.Overdrive(L_Chan, dist);
				//dirt.Overdrive(R_Chan, dist);

				///////////////////////////////////////////////////////////////////////////	
				// Limiter
				///////////////////////////////////////////////////////////////////////////	
					
				limiter.setThresh(thresh);
					limiter.setAttack(1.0);
					limiter.setRelease( 10.0 );
					double gred = limiter.process(L_Chan,R_Chan);
					//int random_integer = (rand()%12)+1;
					
					SetParameter(kParam_Seventeen, gred);
				

				///////////////////////////////////////////////////////////////////////////
				//direct/Cabinet ratio
				///////////////////////////////////////////////////////////////////////////
					
				bassAmp::pmCabinet(L_Chan, R_Chan,12.0, cabinetMix);
				cab.modelNum=cabinetSetting;
					cab.switch_model(cabinetSetting);
					
				///////////////////////////////////////////////////////////////////////////	
				// Equalizer - pm_3Band_EQ class
				///////////////////////////////////////////////////////////////////////////
				
				//Adjustable high and low threshold
					tbEQL.setFreq(&eq3L,int(filter1_freq),int(filter2_freq));
					eq3L.lg = dB2linear(filter1_level); 
					eq3L.mg = dB2linear(filter2_level); 
					eq3L.hg = dB2linear(filter3_level); 
					
					tbEQR.setFreq(&eq3R,int(filter1_freq),int(filter2_freq));
					eq3R.lg = dB2linear(filter1_level); 
					eq3R.mg = dB2linear(filter2_level); 
					eq3R.hg = dB2linear(filter3_level); 
					
					L_Chan = tbEQL.do_3band(&eq3L,L_Chan);
					R_Chan = tbEQR.do_3band(&eq3R,R_Chan);

				///////////////////////////////////////////////////////////////////////////	
				// Chorus
				///////////////////////////////////////////////////////////////////////////
					
					bassAmp::pmChorus(L_Chan, R_Chan, chorusDelay,chorusRate, chorusLevel);
				
					
				///////////////////////////////////////////////////////////////////////////
				// Delay
				///////////////////////////////////////////////////////////////////////////
				// Local
					bassAmp::pmDelay(L_Chan, R_Chan, delayTime, delayLevel);
					
					
				///////////////////////////////////////////////////////////////////////////	
				// to output
				///////////////////////////////////////////////////////////////////////////	
				*dst1 = L_Chan;
				*dst2 = R_Chan; 
				
			}else{
				// bypass
				*dst1 = *src1; 
				*dst2 = *src2; 
			}
	   }

	   return noErr;
}

double bassAmp::dB2linear(double dbValue) {
	static const double DB_2_LOG = 0.11512925464970228420089957273422;	// ln( 10 ) / 20
	return exp( dbValue * DB_2_LOG );
}

void bassAmp::pmCabinet(Float32 &inL, Float32 &inR, Float32 gain, Float32 wetDryLevel)

{
	
	float wetDry = -1 * ((wetDryLevel / 100.) - 1);	// percent to ratio - inverted
	//if (wetDry<=0) wetDry=0;
	float cabinet = sqrt(wetDry), direct = sqrt(1.0 - wetDry);
	
	cab.inputGain= gain;
	cab.input=inL;
	cab.run(int(1));
	
	float outputL = cabinet * inL ; //
	outputL += cab.outputProcessed * direct;
	inL = outputL;
	//inR = outputL;
	
	//right side - not used in this version

	cab.input=inR;
	cab.run(int(1));
	
	float outputR = cabinet * inR ; //
	outputR += cab.outputProcessed * direct;
	inR = outputR;
	
}


void bassAmp::fb_Distortion(Float32 &inL, Float32 &inR, Float32 threshold)
{
	/*
	int sample = int(inL);
	
	if (sample > threshold){
		sample = threshold;
	}
	if(sample < -threshold){
		sample = -threshold;
	inL = Float32(sample);
	}
	
	*/
	
	if (threshold <=0) threshold=0.01;
	
	if (inL > threshold || inL < -threshold)
	{

	
		inL= fabs(fabs(fmod(inL - threshold, threshold*4)) - threshold*2) - threshold;
		inR= fabs(fabs(fmod(inR - threshold, threshold*4)) - threshold*2) - threshold;
		

		// modified
		//inL=fabs(fabs(fabs(fmod(inL - threshold, threshold*8)) - threshold*4) - threshold*2) - threshold;
		//inR=fabs(fabs(fabs(fmod(inR - threshold, threshold*8)) - threshold*4) - threshold*2) - threshold;
		
	
	}
	 
	 

}


void bassAmp::pmChorus(Float32 &inL, Float32 &inR, Float32 ChDelayTime, float chRate, Float32 wetDryLevel)
{
	//single parameter values
	float	level;
	int		readIndex;
	float phaseGain = 1.0; 
	

	chorusLFO.setRate(chRate);
	chorusLFO.setSampleRate(CurrentSampleRate);
	float randNum = chorusLFO.tick();// get lfo value
	
	int frames1 = int((ChDelayTime * randNum ) * GetSampleRate()) ; //delay frames , delay time changes with lfoRate
	if (frames1 < 1) frames1 = 1;
	else if (frames1 >= mMaxDelayFrames_Chorus) frames1 = mMaxDelayFrames_Chorus - 1;
	
	
	level = phaseGain;
	readIndex = NormalizeIndex(mWriteIndex_Chorus - frames1) ;	// read head lags write head...
	
	// intensity
	float wetDry = -1 * ((wetDryLevel / 100.) - 1);	// percent to ratio - inverted
	float wet = sqrt(wetDry), dry = sqrt(1.0 - wetDry);
	
	
	// original for loop start//////////
	
	//write to audio buffer index
	mChorusBufferL[mWriteIndex_Chorus] = inL;
	mChorusBufferR[mWriteIndex_Chorus] = inR;
	mWriteIndex_Chorus = (mWriteIndex_Chorus + 1) % mMaxDelayFrames_Chorus;
	
	float outputL = dry * inL ; //
	float outputR = dry * inR ; //
	
	long readIndex1 = readIndex;
	float delayReadL = mChorusBufferL[readIndex1] ;//
	float delayReadR = mChorusBufferR[readIndex1] ;
	
	readIndex = (readIndex1 + 1) % mMaxDelayFrames_Chorus;
	
	outputL += delayReadL  * level * wet; //distortion on wet side
	outputR += delayReadR  * level * wet;
	
	inL = outputL; 
	inR = outputR; 
		
		
}

void bassAmp::pmDelay(Float32 &inL, Float32 &inR, Float32 delayTime, Float32 wetDryLevel)
{
	
	//single parameter values
	float	level;
	int		readIndex;
	float phaseGain = 1.0; 
	
	int frames1 = int(delayTime  * GetSampleRate()); //delay frames
	if (frames1 < 1) frames1 = 1;
	else if (frames1 >= mMaxDelayFrames) frames1 = mMaxDelayFrames - 1;
	
	
	level = phaseGain;
	readIndex = NormalizeIndex(mWriteIndex - frames1);	// read head lags write head...
	
	// intensity
	float wetDry = -1 * ((wetDryLevel / 100.) - 1);	// percent to ratio - inverted
	float wet = sqrt(wetDry), dry = sqrt(1.0 - wetDry);

	// original for loop start//////////
	
			   //write to audio buffer index
			   mDelayBufferL[mWriteIndex] = inL;
			   mDelayBufferR[mWriteIndex] = inR;
			   mWriteIndex = (mWriteIndex + 1) % mMaxDelayFrames;
			   
			   float outputL = dry * inL ;
			   float outputR = dry * inR ;
			   
			   int readIndex1 = readIndex;
			   float delayReadL = mDelayBufferL[readIndex1];
			   float delayReadR = mDelayBufferR[readIndex1];
			   
			   readIndex = (readIndex1 + 1) % mMaxDelayFrames;
			   
			   outputL += delayReadL * level * wet ;
			   outputR += delayReadR * level * wet ;
			   
			   inL = outputL; 
			   inR = outputR; 
	
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Filter::GetPresets
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		bassAmp::GetPresets (		CFArrayRef * 		outData) const
{
	// this is used to determine if presets are supported 
	// which in this unit they are so we implement this method!
	if (outData == NULL) return noErr;
	
	CFMutableArrayRef theArray = CFArrayCreateMutable (NULL, kNumberPresets, NULL);
	for (int i = 0; i < kNumberPresets; ++i) {
		CFArrayAppendValue (theArray, &kPresets[i]);
    }
    
	*outData = (CFArrayRef)theArray;	// client is responsible for releasing the array
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Filter::NewFactoryPresetSet
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus	bassAmp::NewFactoryPresetSet (const AUPreset & inNewFactoryPreset)
{
	SInt32 chosenPreset = inNewFactoryPreset.presetNumber;
	
	for(int i = 0; i < kNumberPresets; ++i)
	{
		if(chosenPreset == kPresets[i].presetNumber)
		{
			//it would also be possible to use chosenPreset as an index into an array (if you publish the preset
			// numbers as indices in the GetPresets() method)
			
			switch(chosenPreset)
			{
				
				//set preset parameters here
				
				case kPreset_One:
					
					SetParameter(kParam_Two, 1.0 ); //gain
					SetParameter(kParam_Three, 0.74 );//dirt
					SetParameter(kParam_Four, -14.0 );//limit
					SetParameter(kParam_Five, 55 );//cabinet mix
					
					SetParameter(kParam_Six, 2800);//eq low
					SetParameter(kParam_Seven, 6575 );//eq1 high
					SetParameter(kParam_Eight, 5.0 );//eq 1 level
					SetParameter(kParam_Nine, 9.0);//eq2 level
					SetParameter(kParam_Ten, 0.0);//eq3 level
					
					SetParameter(kParam_Eleven, 0.015);//chorus delay time
					SetParameter(kParam_Twelve, 0.05);//chorus rate
					SetParameter(kParam_Thirteen, 95.0);//chorus mix
					
					SetParameter(kParam_Fourteen, 0.13);//delay time
					SetParameter(kParam_Fifteen, 100.0);//delay mix
					
					SetParameter(kParam_Sixteen_Indexed, 5);//cabiniet type
					
					
					break;
					
				case kPreset_Two:
					SetParameter(kParam_Two, 0.83 ); //gain
					SetParameter(kParam_Three, 0.0 );//dirt
					SetParameter(kParam_Four, -16.0 );//limit
					SetParameter(kParam_Five, 53 );//cabinet mix
					
					SetParameter(kParam_Six, 3261);//eq low
					SetParameter(kParam_Seven, 7095 );//eq1 high
					SetParameter(kParam_Eight, 1.0 );//eq 1 level
					SetParameter(kParam_Nine, -8.0);//eq2 level
					SetParameter(kParam_Ten, -4.0);//eq3 level
					
					SetParameter(kParam_Eleven, 0.014);//chorus delay time
					SetParameter(kParam_Twelve, 0.13);//chorus rate
					SetParameter(kParam_Thirteen, 67.0);//chorus mix
					
					SetParameter(kParam_Fourteen, 0.13);//delay time
					SetParameter(kParam_Fifteen, 93.0);//delay mix
					
					SetParameter(kParam_Sixteen_Indexed, 4);//cabiniet type
				
					break;
				case kPreset_Three:
					SetParameter(kParam_Two, 1.2 ); //gain
					SetParameter(kParam_Three, 0.24 );//dirt
					SetParameter(kParam_Four, -12.0 );//limit
					SetParameter(kParam_Five, 69 );//cabinet mix

					SetParameter(kParam_Six, 3261);//eq low
					SetParameter(kParam_Seven, 5428 );//eq1 high
					SetParameter(kParam_Eight, 1.0 );//eq 1 level
					SetParameter(kParam_Nine, 2.0);//eq2 level
					SetParameter(kParam_Ten, -2.0);//eq3 level
					
					SetParameter(kParam_Eleven, 0.02);//chorus delay time
					SetParameter(kParam_Twelve, 0.02);//chorus rate
					SetParameter(kParam_Thirteen, 100.0);//chorus mix
					SetParameter(kParam_Fourteen, 0.25);//delay time
					SetParameter(kParam_Fifteen, 100.0);//delay mix
					SetParameter(kParam_Sixteen_Indexed, 3);//cabiniet type

					break;
					
				case kPreset_Four:
				
					
					
					
					
					SetParameter(kParam_Two, 1.0 ); //gain
					SetParameter(kParam_Three, 0.0 );//dirt
					SetParameter(kParam_Four, -11.0 );//limit
					SetParameter(kParam_Five, 47 );//cabinet mix
					
					SetParameter(kParam_Six, 2328);//eq low
					SetParameter(kParam_Seven, 8073 );//eq1 high
					SetParameter(kParam_Eight, 5.0 );//eq 1 level
					SetParameter(kParam_Nine, -10.0);//eq2 level
					SetParameter(kParam_Ten, 4.0);//eq3 level
					
					SetParameter(kParam_Eleven, 0.019);//chorus delay time
					SetParameter(kParam_Twelve, 0.1);//chorus rate
					SetParameter(kParam_Thirteen, 86.0);//chorus mix
					SetParameter(kParam_Fourteen, 0.14);//delay time
					SetParameter(kParam_Fifteen, 83.0);//delay mix
					SetParameter(kParam_Sixteen_Indexed, 2);//cabiniet type
					break;
					
					
					
			}
            
           SetAFactoryPresetAsCurrent (kPresets[i]);
			return noErr;
		}
	}
	
	return kAudioUnitErr_InvalidPropertyValue;
}




