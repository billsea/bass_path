/* pm_SliderKnob */

#import <Cocoa/Cocoa.h>

@interface pm_SliderKnob : NSView
{
	NSBezierPath * circleBase;
	NSBezierPath * circleInner;
	NSBezierPath * settingPath;
	NSImage *bgImage;
	NSImage *dialImage;
	
	float dialSize;
	float maxYcoord;
	
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
	
	int mouseClick;
	
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
- (float)imgSizeWidthValue;
- (float)imgSizeHeightValue;
- (BOOL)copyable;
- (SEL)action;
- (void)setAction:(SEL)newAction;
- (id)target;
- (void)setTarget:(id)newTarget;


@end
