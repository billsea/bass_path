/* pm_AudioKnob2 */

#import <Cocoa/Cocoa.h>

@interface pm_AudioKnob2 : NSView
{
	
	
	NSBezierPath * circleBase;
	NSBezierPath * circleInner;
	NSBezierPath * settingPath;
	NSImage *bgImage;
	NSImage *dialImage;
	
	float maxXcoord;
	
	NSPoint currentPoint;
	NSPoint offsetPoint;
	
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
	float dialSize;
	
	int mouseClick;
	
	NSShadow *shadow;
	NSMutableDictionary *attributes; // for string attributes(fonts etc...)
}

- (void)prepareAttributes;
- (IBAction)setAngle:(id)sender;
- (void)setCurrentValue:(float)curVal;
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
