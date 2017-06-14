/*
*	File:		CocoaView.h
  *
  *	Version:	1.0
  *
  *	Created:	11/22/2006
  *
  *	Copyright:  Copyright © 2006 primo-media.com, All Rights Reserved
  *
  *
*/

#import <Cocoa/Cocoa.h>
#import <AudioUnit/AudioUnit.h>
#import <AudioToolbox/AudioToolbox.h>
#import <audioControls/audioControls.h>


//Notes:
// the audioControls.framework is referenced from the users home/Library/Frameworks directory.
// this must be added at install time or the program will fail

//#error Rename ALL UI classes with a unique name to avoid namespace collisions
/************************************************************************************************************/
/* NOTE: It is important to rename ALL ui classes when using the XCode Audio Unit with Cocoa View template	*/
/*		 Cocoa has a flat namespace, and if you use the default filenames, it is possible that you will		*/
/*		 get a namespace collision with classes from the cocoa view of a previously loaded audio unit.		*/
/*		 We recommend that you use a unique prefix that includes the manufacturer name and unit name on		*/
/*		 all objective-C source files. You may use an underscore in your name, but please refrain from		*/
/*		 starting your class name with an undescore as these names are reserved for Apple.					*/
/*  Example  : AppleDemoFilter_UIView AppleDemoFilter_ViewFactory											*/
/************************************************************************************************************/

@interface bassAmp_CocoaView : NSView
{
    // IB Members
	IBOutlet NSButton *				uiBypassButton;
	IBOutlet NSView *				uiParam1knob;
	IBOutlet NSView *				uiParam2knob;
	IBOutlet NSView *				uiParam3knob;
	IBOutlet NSView *				uiParam4knob;
	IBOutlet NSView *				uiParam5knob;
	IBOutlet NSView *				uiParam6knob;
	IBOutlet NSView *				uiParam7SliderKnob;
	IBOutlet NSView *				uiParam8SliderKnob;
	IBOutlet NSView *				uiParam9SliderKnob;

	IBOutlet NSView *				uiParam10knob;
	IBOutlet NSView *				uiParam11knob;
	IBOutlet NSView *				uiParam12knob;
	IBOutlet NSView *				uiParam13knob;
	IBOutlet NSView *				uiParam14knob;
	
	IBOutlet NSStepper *			uiCabinetSelector;
	IBOutlet NSLevelIndicator *		uiLimiterMeter;
	NSTimer 			*			gLimiterMeterTimer;
	
	IBOutlet NSTextField *			uiParam1TextField;//level display
	IBOutlet NSTextField *			uiParam2TextField;//cabinet type
		
	IBOutlet NSImageView *			imgView;
    // Other Members
    AudioUnit						mAU;
    AUParameterListenerRef			mParameterListener;
	
	NSColor	*						mBackgroundColor;	// the background color (pattern) of the view
	NSImage *						backgroundImage;
	
	IBOutlet NSWindow * regCustomSheet;
	IBOutlet NSTextField * regNumberField;
	IBOutlet NSButton * regLater;
	IBOutlet NSTextField * remainingEvaluationTime;
	IBOutlet NSButton * butQuit;
	
	int  adjustState;

}
- (IBAction) openPrimoSite:(id)sender;

//typedef CALLBACK_API_C( ComponentResult , AudioUnitGetParameterProc  )(void *inComponentStorage, AudioUnitParameterID inID, AudioUnitScope  inScope, AudioUnitElement inElement, Float32 *outValue);
	


#pragma mark ____ PUBLIC FUNCTIONS ____
- (void)setAU:(AudioUnit)inAU;
- (double)dB2linear:(double)dbValue;
- (double)lin2dB:(double)lin;
- (void)mouseDown:(NSEvent *)event;
- (void)mouseUp:(NSEvent *)event;
- (int) getAdjustState;
- (void)showRegistrationSheet:(BOOL)useCount;
- (void)evaluationCheck;


#pragma mark ____ INTERFACE ACTIONS ____
- (IBAction)unloadApp:(id)sender;
- (IBAction)endRegistrationWindow:(id)sender;
- (IBAction)iaOnStatus:(id)sender;
- (IBAction)iaParam1Changed:(id)sender;
- (IBAction)iaParam2Changed:(id)sender;
- (IBAction)iaParam3Changed:(id)sender;
- (IBAction)iaParam4Changed:(id)sender;
- (IBAction)iaParam5Changed:(id)sender;
- (IBAction)iaParam6Changed:(id)sender;
- (IBAction)iaParam7Changed:(id)sender;

- (IBAction)iaParam8Changed:(id)sender;
- (IBAction)iaParam9Changed:(id)sender;
- (IBAction)iaParam10Changed:(id)sender;
- (IBAction)iaParam11Changed:(id)sender;
- (IBAction)iaParam12Changed:(id)sender;
- (IBAction)iaParam13Changed:(id)sender;
- (IBAction)iaParam14Changed:(id)sender;
- (IBAction)iaParam15Changed:(id)sender;

- (void)displayLimiterGR:(id)sender;
- (IBAction)serialNumberEntry:(id)sender;
- (IBAction) openRegistrationSite:(id)sender;

#pragma mark ____ PRIVATE FUNCTIONS
- (void)_synchronizeUIWithParameterValues;
- (void)_addListeners;
- (void)_removeListeners;



#pragma mark ____ LISTENER CALLBACK DISPATCHEE ____
- (void)_parameterListener:(void *)inObject parameter:(const AudioUnitParameter *)inParameter value:(Float32)inValue;

@end
