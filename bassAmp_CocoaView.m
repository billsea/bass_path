/*
*	File:		CocoaView.m
*	
*	Version:	1.0
* 
*	Created:	11/22/2006
*	
*	Copyright:  Copyright © 2006 primo-media.com, All Rights Reserved
* 
*
*/

#import "bassAmp_CocoaView.h"


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
	kParam_Fourteen ,
	kParam_Fifteen,
	kParam_Sixteen_Indexed,
	kParam_Seventeen,
	kNumberOfParameters=17
};

#pragma mark ____ LISTENER CALLBACK DISPATCHER ____
AudioUnitParameter parameter[] = {	
	{ 0, kParam_One_Indexed, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Two, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Three, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Four, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Five, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Six, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Seven, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Eight, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Nine, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Ten, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Eleven, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Twelve, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Thirteen, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Fourteen, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Fifteen, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Sixteen_Indexed, kAudioUnitScope_Global, 0 },
	{ 0, kParam_Seventeen, kAudioUnitScope_Global, 0 },
};

void ParameterListenerDispatcher (void *inRefCon, void *inObject, const AudioUnitParameter *inParameter, Float32 inValue) {
	bassAmp_CocoaView *SELF = (bassAmp_CocoaView *)inRefCon;
    
    [SELF _parameterListener:inObject parameter:inParameter value:inValue];
	
}

@implementation bassAmp_CocoaView




-(void) awakeFromNib {
	//NSString *path = [[NSBundle bundleForClass: [bassAmp_CocoaView class]] pathForImageResource: @"SectionPatternLight"];
	//NSImage *pattern = [[NSImage alloc] initByReferencingFile: path];
	
	
	// Check Evaluation time and registration id
	[self evaluationCheck];
	
	
	mBackgroundColor = [NSColor whiteColor];//[[NSColor colorWithPatternImage: [pattern autorelease]] retain];
	backgroundImage =[NSImage imageNamed:@"brushedMetal.gif"];
	
	// setup a timer to call limiter meter display 
    gLimiterMeterTimer = [[NSTimer scheduledTimerWithTimeInterval:0.01		// interval, 0.01 seconds
												 target:self
											   selector:@selector(displayLimiterGR:)		// call this method
											   userInfo:nil
												repeats:YES] retain];					// repeat until we cancel it
	
	
		
}
/////////////////////////////////////////////////////////////////////////////////
////////////////////// Evaluation and Registration Functions ////////////////////
/////////////////////////////////////////////////////////////////////////////////
- (void)evaluationCheck
{
	
	//check evaluation period and registration (rgLic.plist)
	int evalLimit = 14; // evaluation days - limit
	NSString * regInitDate;
	long expiredDays;
	BOOL evalExpired = FALSE;
	NSString *regPath = [[NSBundle bundleWithIdentifier: @"com.primomedia.audiounit.bassAmp"] pathForResource:@"rgLic" ofType:@"plist"];
	NSMutableDictionary * regDict;
	regDict = [NSMutableDictionary dictionaryWithContentsOfFile:regPath];
	
	//show remaining evaluation time, allow for serial number entry, or kill.
	NSString * serialNum = [[NSString alloc] initWithString:@"bampv100x757832"]; //this is also set in (IBAction)serialNumberEntry:(id)sender
	
	if ([ [regDict objectForKey:@"serial"] isEqual: serialNum])
	{
		//NSLog(@"serial number is ok");
		
	}else{
		
		NSCalendarDate * today;
		today = [NSCalendarDate date];
		NSString * noVal = [[NSString alloc] initWithString:@"???"];
		
		//set day of year value for regInit in plist of first use
		if ([[regDict objectForKey:@"regInit"] isEqual:noVal]){
			
			[regDict setObject:today forKey:@"regInit"];
			[regDict writeToFile:regPath atomically:nil];
			
			regInitDate= [regDict objectForKey:@"regInit"]  ;
			
			NSDate * dInitDate = regInitDate;// format: 2007-01-14 17:15:30 -0800
				
				long tSecs1 = [today timeIntervalSinceDate:dInitDate];
				expiredDays = tSecs1/86400;
			
		}else{
		
			regInitDate = [regDict objectForKey:@"regInit"];
			
			//this seems odd but it works...
			NSDate * dInitDate = regInitDate;// format: 2007-01-14 17:15:30 -0800
	
			long tSecs = [today timeIntervalSinceDate:dInitDate];
			expiredDays = tSecs/86400;
			//NSLog(@"Interval since init time(days) = %d", expiredDays); 	 
		}
		
		if (expiredDays > evalLimit){
			evalExpired = TRUE;
			//NSLog(@"Expired = true"); 
		}
		
		int remainingEvalDays = evalLimit - expiredDays;
		[remainingEvaluationTime setIntValue:remainingEvalDays];
		
		//Debug
		//NSLog(@"Init date = %@", regInitDate); 
		//NSLog(@"serial number value = %@", [regDict objectForKey:@"serial"]); 
	   // NSLog(@"today = %d", [today dayOfYear]); 
		
		// Show registration sheet
		[self showRegistrationSheet:evalExpired];
		
	}
	
}

- (void)showRegistrationSheet:(BOOL)expired

	// User has asked to see the custom display. Display it.
{
    if (!regCustomSheet)
		//Check the myCustomSheet instance variable to make sure the custom sheet does not already exist. 
        [NSBundle loadNibNamed: @"registerBP" owner: self];

	// Registration sheet is displayed, and application is suspended until the user enters a command
	
	//if evaluation expired, disable "Register later" button

	if (expired == TRUE)
	{
		[regLater setHidden:TRUE];
		
		[butQuit setHidden:FALSE];

	}
	
	
	[NSApp beginSheet:regCustomSheet
	   modalForWindow:[self window]
		modalDelegate:nil
	   didEndSelector:nil
		  contextInfo:nil];
	
	[NSApp runModalForWindow: regCustomSheet];
	
    // Dialog is up here.
    [NSApp endSheet: regCustomSheet];
    [regCustomSheet orderOut: self];
	
}

- (IBAction)serialNumberEntry:(id)sender
{
	NSString * serialNum = [[NSString alloc] initWithString:@"bampv100x757832"];//valid registration number
	if ([ [regNumberField stringValue] isEqual: serialNum])
	{
		//enter serial number into registration plist
		NSString *regPath = [[NSBundle bundleWithIdentifier: @"com.primomedia.audiounit.bassAmp"] pathForResource:@"rgLic" ofType:@"plist"];
		NSMutableDictionary * regDict;
		regDict = [NSMutableDictionary dictionaryWithContentsOfFile:regPath];
		[regDict setObject:[regNumberField stringValue] forKey:@"serial"];
		[regDict writeToFile:regPath atomically:nil]; //write back to rgLic.plist file
	
		int serNum = NSRunAlertPanel(@"Registration completed!", @"Thanks for Registering bassPath!",
									 nil, nil,nil,nil);
		
		[NSApp endSheet:regCustomSheet returnCode:1];
		
	}else{
		//Show error on registration
		int serNum = NSRunAlertPanel(@"Registration entry error", @"The registration number you have entered is not valid, please confirm your registration number re-enter.",
									 nil, nil,nil,nil);
		
	}
}

- (IBAction)endRegistrationWindow:(id)sender
{
	//return to normal event handling
	
	[NSApp endSheet:regCustomSheet returnCode:1];
	
}

- (IBAction)unloadApp:(id)sender;
{
	
	[NSApp endSheet:regCustomSheet returnCode:1];
	[uiBypassButton setEnabled:FALSE];
	//[self setHidden:TRUE]; //hide main view
	//bypass
	float intValue = 1;
	
	AUParameterSet(mParameterListener, NULL, &parameter[0], intValue, 0);
	
	int serNum = NSRunAlertPanel(@"Thanks for evaluating bassPath!", @"bassPath will now run in bypass mode. Please register today!",
								 nil, nil,nil,nil);
	
	
	/* ////////////////////////////////////////////////
	   // Attempt to use component manager to unload the audio unit.
	   // identify audio unit component
		
		ComponentDescription desc;
	
	desc.componentType = kAudioUnitType_Effect;            
	desc.componentSubType = 'Bamp';           
	desc.componentManufacturer = 'Prim';  
	desc.componentFlags = 0;                               
	desc.componentFlagsMask = cmpIsMissing;                 
	long numComp =  CountComponents(&desc);
	NSLog(@"component count is = %d",numComp ); 
	
	long componentID;	
	
	componentID = (FindNextComponent(componentID, &desc));  //
	NSLog(@"component ID is = %d",componentID ); 
	
	long cinsCount = CountComponentInstances ((Component)componentID);
	NSLog(@"component instances  = %d",cinsCount); 
	
	ComponentInstance theInst = OpenDefaultComponent (desc.componentType,desc.componentSubType);
	
	//close
	OSErr err = 	CloseComponent (theInst);
	if (err==noErr){ NSLog(@"no error code=%d", (int)err );}else{  NSLog(@"error!!! code=%d", (int)err );}
    */////////////////////////////////////////////////
	
}



/////////////////////////////////////////////////////////////////////////////////
//////// End   ----- Evaluation and Registration Functions -----End  ////////////
/////////////////////////////////////////////////////////////////////////////////


#pragma mark ____ (INIT /) DEALLOC ____
- (void)dealloc {
    [self _removeListeners];
	[mBackgroundColor release];
    [super dealloc];
}

#pragma mark ____ PUBLIC FUNCTIONS ____
- (void)setAU:(AudioUnit)inAU {
	// remove previous listeners
	if (mAU) [self _removeListeners];
	mAU = inAU;
    
	// add new listeners
	[self _addListeners];
	
	// initial setup
	[self _synchronizeUIWithParameterValues];
}

- (void)drawRect:(NSRect)rect
{
	
	[mBackgroundColor set];
	//[backgroundImage compositeToPoint:NSMakePoint(0.0,0.0) operation:NSCompositeSourceOver];
	NSRectFill(rect);		// this call is much faster than using NSBezierPath, but it doesn't handle non-opaque colors
	
	[super drawRect: rect];	// we call super to draw all other controls after we have filled the background
}

#pragma mark ____ INTERFACE ACTIONS ____

// On/Off vocal reduction switch
- (IBAction)iaOnStatus:(id)sender{
	
	int butState = [sender state];
	
	if (butState==1){
		[uiParam1TextField setStringValue:@"Bypass"];
	}else{
		 [uiParam1TextField setStringValue:@"Active"];
	}
	
	
	int intValue = butState;
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[0], intValue, 0) == noErr,
                @"[bassAmp_CocoaView iaOnStatus:] AUParameterSet()");
		
}


- (IBAction)iaParam1Changed:(id)sender {
    float floatValue = [sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[1], (Float32)floatValue, 0) == noErr,
                @"[bassAmp_CocoaView iaParam1Changed:] AUParameterSet()");
	adjustState = 1;
	
	NSNumber * theValue = [[NSNumber alloc] initWithFloat:floatValue];
	NSNumberFormatter * numFormat = [[NSNumberFormatter alloc] autorelease];
	[numFormat setFormat:@"#.##"];
	
    if (sender == uiParam1knob) {
      [uiParam1TextField setStringValue: [numFormat stringFromNumber:theValue]];
    } else {
		[uiParam1TextField setStringValue: [numFormat stringFromNumber:theValue]];
    }
}
- (IBAction)iaParam2Changed:(id)sender {
    float floatValue = [sender getKnobLinearValue];
	NSAssert(AUParameterSet(mParameterListener, sender, &parameter[2], (Float32)floatValue, 0) == noErr,
                @"[bassAmp_CocoaView iaParam2Changed:] AUParameterSet()");
	adjustState = 1;
	
	NSNumber * theValue = [[NSNumber alloc] initWithFloat:floatValue];
	NSNumberFormatter * numFormat = [[NSNumberFormatter alloc] autorelease];
	[numFormat setFormat:@"#.##"];
	
    if (sender == uiParam2knob) {
        [uiParam1TextField setStringValue: [numFormat stringFromNumber:theValue]];
    } else {
		[uiParam1TextField setStringValue: [numFormat stringFromNumber:theValue]];
    }
	
}

- (IBAction)iaParam3Changed:(id)sender {
    float floatValue =[sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[3], (Float32)floatValue, 0) == noErr,
                @"[bassAmp_CocoaView iaParam3Changed:] AUParameterSet()");
	adjustState = 1;
    if (sender == uiParam3knob) {
		  [uiParam1TextField setIntValue:floatValue];
    } else {

	 [uiParam1TextField setIntValue:floatValue];
    }
}
	
	- (IBAction)iaParam4Changed:(id)sender {
		float floatValue =[sender getKnobLinearValue];
		NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[4], (Float32)floatValue, 0) == noErr,
					@"[bassAmp_CocoaView iaParam4Changed:] AUParameterSet()");
		adjustState = 1;
		if (sender == uiParam4knob) {
			 [uiParam1TextField setIntValue:floatValue];
		} else {

			 [uiParam1TextField setIntValue:floatValue];
		}
}

- (IBAction)iaParam5Changed:(id)sender {
	float floatValue =[sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[5], (Float32)floatValue, 0) == noErr,
				@"[bassAmp_CocoaView iaParam5Changed:] AUParameterSet()");
	adjustState = 1;
	if (sender == uiParam5knob) {
		 [uiParam1TextField setIntValue:floatValue];
	} else {

		 [uiParam1TextField setIntValue:floatValue];
	}
}

- (IBAction)iaParam6Changed:(id)sender {
	float floatValue =[sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[6], (Float32)floatValue, 0) == noErr,
				@"[bassAmp_CocoaView iaParam6Changed:] AUParameterSet()");
	adjustState = 1;
	
	if (sender == uiParam6knob) {
		 [uiParam1TextField setIntValue:floatValue];
	} else {
			//[uiParam1TextField setFloatValue:floatValue];
	}
}

- (IBAction)iaParam7Changed:(id)sender {
	float floatValue =[sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[7], (Float32)floatValue, 0) == noErr,
				@"[bassAmp_CocoaView iaParam7Changed:] AUParameterSet()");
	adjustState = 1;
	
	if (sender == uiParam7SliderKnob) {
		 [uiParam1TextField setIntValue:floatValue];
	} else {

		 [uiParam1TextField setIntValue:floatValue];
	}
}


- (IBAction)iaParam8Changed:(id)sender {
	float floatValue =[sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[8], (Float32)floatValue, 0) == noErr,
				@"[bassAmp_CocoaView iaParam8Changed:] AUParameterSet()");
	adjustState = 1;
	
	if (sender == uiParam8SliderKnob) {
		 [uiParam1TextField setIntValue:floatValue];
	} else {

		 [uiParam1TextField setIntValue:floatValue];
	}
}


- (IBAction)iaParam9Changed:(id)sender {
	float floatValue =[sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[9], (Float32)floatValue, 0) == noErr,
				@"[bassAmp_CocoaView iaParam9Changed:] AUParameterSet()");
	adjustState = 1;
	if (sender == uiParam9SliderKnob) {
		 [uiParam1TextField setIntValue:floatValue];
	} else {

		 [uiParam1TextField setIntValue:floatValue];
	}
	
	
}
- (IBAction)iaParam10Changed:(id)sender {
	float floatValue =[sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[10], (Float32)floatValue, 0) == noErr,
				@"[bassAmp_CocoaView iaParam10Changed:] AUParameterSet()");
	adjustState = 1;
	NSNumber * theValue = [[NSNumber alloc] initWithFloat:floatValue];

	if (sender == uiParam10knob) {
		[uiParam1TextField setStringValue:  [NSString stringWithFormat:@"%.3f", [theValue floatValue]]];
	} else {

		[uiParam1TextField setStringValue:  [NSString stringWithFormat:@"%.3f", [theValue floatValue]]];
	}
	
	
}
- (IBAction)iaParam11Changed:(id)sender {
	float floatValue =[sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[11], (Float32)floatValue, 0) == noErr,
				@"[bassAmp_CocoaView iaParam11Changed:] AUParameterSet()");
	adjustState = 1;
	NSNumber * theValue = [[NSNumber alloc] initWithFloat:floatValue];
	NSNumberFormatter * numFormat = [[NSNumberFormatter alloc] autorelease];
	[numFormat setFormat:@"#.##"];
	
	if (sender == uiParam11knob) {
		[uiParam1TextField setStringValue: [numFormat stringFromNumber:theValue]];
	} else {

		[uiParam1TextField setStringValue: [numFormat stringFromNumber:theValue]];
	}
}
- (IBAction)iaParam12Changed:(id)sender {
	float floatValue =[sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[12], (Float32)floatValue, 0) == noErr,
				@"[bassAmp_CocoaView iaParam12Changed:] AUParameterSet()");
	adjustState = 1;
	if (sender == uiParam12knob) {
		[uiParam1TextField setIntValue:floatValue];
	} else {

		[uiParam1TextField setIntValue:floatValue];
	}
}
- (IBAction)iaParam13Changed:(id)sender {
	float floatValue =[sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[13], (Float32)floatValue, 0) == noErr,
				@"[bassAmp_CocoaView iaParam13Changed:] AUParameterSet()");
	adjustState = 1;

	NSNumber * theValue = [[NSNumber alloc] initWithFloat:floatValue];
	NSNumberFormatter * numFormat = [[NSNumberFormatter alloc] autorelease];
	[numFormat setFormat:@"$##.##"];

	if (sender == uiParam13knob) {
		[uiParam1TextField setStringValue: [numFormat stringFromNumber:theValue]];
	} else {
		//[uiParam13knob setFloatValue:floatValue];
	[uiParam1TextField setStringValue: [numFormat stringFromNumber:theValue]];
	}
}
- (IBAction)iaParam14Changed:(id)sender {
	float floatValue =[sender getKnobLinearValue];
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[14], (Float32)floatValue, 0) == noErr,
				@"[bassAmp_CocoaView iaParam14Changed:] AUParameterSet()");
	adjustState = 1;
	if (sender == uiParam14knob) {
		 [uiParam1TextField setIntValue:floatValue];
	} else {
		//[uiParam14knob setFloatValue:floatValue];
	 [uiParam1TextField setIntValue:floatValue];
	}
}

- (IBAction)iaParam15Changed:(id)sender {
	
	NSString * stringValue = [sender stringValue];
	int intValue = [stringValue intValue];

	//NSLog(@"cabinet value ========= %d", intValue);//
	
	NSAssert(	AUParameterSet(mParameterListener, sender, &parameter[15], intValue, 0) == noErr,
				@"[bassAmp_CocoaView iaParam15Changed:] AUParameterSet()");
	
	if (sender == uiCabinetSelector) {
		[uiParam2TextField setIntValue:intValue];
	} else {
		//[uiParam15knob setFloatValue:floatValue];
		[uiParam2TextField setIntValue:intValue];
	}
	
}


//called by timer(initialized in awakeFromNIB function) to display limiter gain reduction
- (void)displayLimiterGR:(id)sender;
{
	float value;
	ComponentResult cr;
	cr = AudioUnitGetParameter(mAU, parameter[16].mParameterID, kAudioUnitScope_Global, 0, &value);
	
	[uiLimiterMeter setFloatValue:[self dB2linear:value] * 20];
	//NSLog(@"meter Display value =  %f", [self dB2linear:value] * 20);//
		
}


- (double)dB2linear:(double)dbValue {
	static const double DB_2_LOG = 0.11512925464970228420089957273422;	// ln( 10 ) / 20
	return exp( dbValue * DB_2_LOG );
}

- (double)lin2dB:(double)lin  {
	static const double LOG_2_DB = 8.6858896380650365530225783783321;	// 20 / ln( 10 )
	return log( lin ) * LOG_2_DB;
}



#pragma mark ____ PRIVATE FUNCTIONS ____
- (void)_addListeners {
	NSAssert (	AUListenerCreate(	ParameterListenerDispatcher, self, 
                                    CFRunLoopGetCurrent(), kCFRunLoopDefaultMode, 0.100, // 100 ms
                                    &mParameterListener	) == noErr,
                @"[bassAmp_CocoaView _addListeners] AUListenerCreate()");
	
    int i;
    for (i = 0; i < kNumberOfParameters; ++i) {
        parameter[i].mAudioUnit = mAU;
        NSAssert (	AUListenerAddParameter (mParameterListener, NULL, &parameter[i]) == noErr,
                    @"[bassAmp_CocoaView _addListeners] AUListenerAddParameter()");
    }
}

- (void)_removeListeners {
    int i;
    for (i = 0; i < kNumberOfParameters; ++i) {
        NSAssert (	AUListenerRemoveParameter(mParameterListener, NULL, &parameter[i]) == noErr,
                    @"[bassAmp_CocoaView _removeListeners] AUListenerRemoveParameter()");
    }
    
	NSAssert (	AUListenerDispose(mParameterListener) == noErr,
                @"[bassAmp_CocoaView _removeListeners] AUListenerDispose()");
}

- (void)_synchronizeUIWithParameterValues {
	Float32 value;
    int i;
    
    for (i = 0; i < kNumberOfParameters; ++i) {
        // only has global parameters
        NSAssert (	AudioUnitGetParameter(mAU, parameter[i].mParameterID, kAudioUnitScope_Global, 0, &value) == noErr,
                    @"[bassAmp_CocoaView synchronizeUIWithParameterValues] (x.1)");
        NSAssert (	AUParameterSet (mParameterListener, self, &parameter[i], value, 0) == noErr,
                    @"[bassAmp_CocoaView synchronizeUIWithParameterValues] (x.2)");
        NSAssert (	AUParameterListenerNotify (mParameterListener, self, &parameter[i]) == noErr,
                    @"[bassAmp_CocoaView synchronizeUIWithParameterValues] (x.3)");
    }
}

- (void)mouseDown:(NSEvent *)event
{
	// adjustState = 0;
	//NSLog(@"endAdjustLevel value = %d",  adjustState);//
}
- (void)mouseUp:(NSEvent *)event
{
	 adjustState = 0;
	//NSLog(@"endAdjustLevel value = %d",  adjustState);//
}

- (int) getAdjustState
{
	return adjustState;
}


#pragma mark ____ LISTENER CALLBACK DISPATCHEE ____
- (void)_parameterListener:(void *)inObject parameter:(const AudioUnitParameter *)inParameter value:(Float32)inValue {
    //inObject ignored in this case.
	
	//this is called when control is released by a "Mouse up"
	if ([self  getAdjustState] != 1) {

	switch (inParameter->mParameterID) {
		case kParam_One_Indexed:
			//NSLog(@"inValue1 value = %f", inValue);//
			[uiBypassButton setIntValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithInt:inValue] stringValue]];
			break;
		case kParam_Two:
			//NSLog(@"inValue2 value = %f", inValue);
			[uiParam1knob setCurrentValue:inValue];
			[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Three:
			//NSLog(@"inValue3 value = %f", inValue);
			[uiParam2knob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Four:
			//NSLog(@" kParam_Four: = %d", (inParameter->mParameterID));
			[uiParam3knob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Five:
			//NSLog(@" kParam_Five: = %d", (inParameter->mParameterID));
			[uiParam4knob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Six:
			//NSLog(@" kParam_Six: value = %d", (inParameter->mParameterID ));
			[uiParam5knob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Seven:
			//NSLog(@" kParam_Seven: = %f", inValue);
			[uiParam6knob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
			
		case kParam_Eight:
			//NSLog(@" kParam_Eight: = %d", (inParameter->mParameterID));
			[uiParam7SliderKnob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Nine:
			//NSLog(@" kParam_Nine: = %d", (inParameter->mParameterID));
			[uiParam8SliderKnob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
	
		case kParam_Ten:
			//NSLog(@" kParam_Ten: = %d", (inParameter->mParameterID));
			[uiParam9SliderKnob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Eleven:
			//NSLog(@" kParam_Eleven: = %d", (inParameter->mParameterID));
			[uiParam10knob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Twelve:
			//NSLog(@"inValue3 value = %f", inValue);
			[uiParam11knob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Thirteen:
			//NSLog(@"inValue3 value = %f", inValue);
			[uiParam12knob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Fourteen:
			//NSLog(@"inValue3 value = %f", inValue);
			[uiParam13knob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Fifteen:
			//NSLog(@"inValue3 value = %f", inValue);
			[uiParam14knob setCurrentValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Sixteen_Indexed:
			//NSLog(@"cabinet value = %f", inValue);
			[uiCabinetSelector setIntValue:inValue];
			[uiParam2TextField setIntValue:inValue];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;
		case kParam_Seventeen:
			//NSLog(@"inValue3 value = %f", inValue);
			[uiLimiterMeter setFloatValue:[self dB2linear:inValue] * 20];
			//[uiParam1TextField setStringValue:[[NSNumber numberWithFloat:inValue] stringValue]];
			break;

	}
	
	}
}

- (IBAction) openPrimoSite:(id)sender

{
	NSString *stringURL = @"http://primo-media.com";
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:stringURL]];
	
}

- (IBAction) openRegistrationSite:(id)sender
{
	
	NSString *stringURL = @"http://primo-media.com/?page_Request=Downloads";
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:stringURL]];
}

@end
