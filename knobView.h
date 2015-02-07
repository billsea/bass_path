/* knobView */

#import <Cocoa/Cocoa.h>

@interface knobView : NSView
{
	
	NSBezierPath * circleBase;
	NSBezierPath * circleInner;
	NSBezierPath * settingPath;
	NSImage *bgImage;
	
	NSPoint currentPoint;
	NSPoint offsetPoint;

	NSRect rectMain;
	NSRect r;
	NSPoint pCenter;

}


- (IBAction)setAngle:(id)sender;
- (float)getKnobLinearValue;

@end
