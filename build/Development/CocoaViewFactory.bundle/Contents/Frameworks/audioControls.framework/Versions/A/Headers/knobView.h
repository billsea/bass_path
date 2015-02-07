/* /////////////////////////////////////////////
knobView Interface builder audio control class
////////////////////////////////////////////////
this is a basic audio knob, with minimum, maximum, and default setting.
Notes: images must be referenced by bundle(see load image in init function). the 
A Framework must also be created for the source classes, when using the custom control, and referenced
by the host application. 
- check the plist files, and cofirm the "CFBundleIdentifier" settings. ONE Important note...the 
	framework plist CFBundleIdentifier string must match this one for referencing images. Was a bit
	tricky referencing images from project to project. - WES 11/21/2006

- Steps to create custom control
- Create a new view project, and add images, actions, targets, etc.( knobView in this case)
- Palletize the custom view(create a new pallet project)(AudioControls in this case)
- make sure all image files, plists, etc are referenced correctly(this is a pain in the ass)
- Create a framework for the source custom view files(audioControls in this case)

	///////////////////////////////////////////////////////////*/

#import <Cocoa/Cocoa.h>

@interface knobView : NSView
{
	
	NSBezierPath * circleBase;
	NSBezierPath * circleInner;
	NSBezierPath * settingPath;
	NSImage *bgImage;
	
	NSPoint currentPoint;
	NSPoint offsetPoint;
	
	float dialSize;
	float maxXcoord;
	
	NSRect rectMain;
	NSRect r;
	NSPoint pCenter;
	
	float testAngle;
	float lastPoint;
	float clickOffset;	
	
	SEL action;
	id	target;
	
	BOOL copyable;
	float maxValue;
	float minValue;
	float defaultValue;
	float minValueOffset;
	float maxValueOffset;
	
	NSShadow *shadow;
	NSMutableDictionary *attributes; // for string attributes(fonts etc...)
}

- (void)prepareAttributes;
- (IBAction)setAngle:(id)sender;
- (float)setCurrentValue:(float)curVal;
- (void) setMinimumValue:(float)minVal;
- (void) setMaximumValue:(float)maxVal;
- (void)setMaxValueOffset:(float)mxValue;
- (void)setMinValueOffset:(float)mnValue;
- (void) setDefaultValue:(float)defaultVal;
- (void) setImageSizeValue:(float)imgVal;
- (float)getKnobLinearValue;
- (void)setCopyable:(BOOL)yn;
- (float)maximumValue;
- (float)minimumValue;
- (float)defaultValue;
- (float)imgSizeValue;
- (BOOL)copyable;
- (SEL)action;
- (void)setAction:(SEL)newAction;
- (id)target;
- (void)setTarget:(id)newTarget;

@end
