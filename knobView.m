#import "knobView.h"
#include <math.h>
#include "bassAmp_CocoaView.h"

@implementation knobView

float testAngle;
float lastPoint;
float clickOffset;

- (id)initWithFrame:(NSRect)frameRect
{
	if ((self = [super initWithFrame:frameRect]) != nil) {
		// Add initialization code here
			NSLog(@"initializing audio knob custom view");
		//center point
		r = [self bounds];
		
		//get knob image
		NSString *knobImagePath = [[NSBundle bundleWithIdentifier: @"com.primomedia.audiounit.bassAmp"] pathForImageResource:@"basicAudioKnob.gif"];
		bgImage = [[NSImage alloc] initByReferencingFile: knobImagePath];; //[NSImage imageNamed:@"Contents/Resources/basicAudioKnob.gif"];
		
	}
	return self;
}
/*
// Coding methods for Creating an Interface builder Palette
// If the view comes from a NIB file, this will be used instead of "initWithFrame"
- (void)encodeWithCoder:(NSCoder *)coder
{
	//Call NSViews's encodeWithCoder: method
	[super encodeWithCoder:coder];
	if ([coder allowsKeyedCoding]) {
		//[coder encodeObject:testAngle forKey:@"angle"];
		//[coder encodeObject:string forKey:@"string"];
		//[coder encodeBool:copyable forKey:@"copyable"];
	} else {
		//[coder encodeObject:testAngle];
		//[coder encodeObject:string];
		//[coder encodeValueOfObjCType:@encode(BOOL)
		//						  at:&copyable];
	}
}

- (id)initWithCoder:(NSCoder *)coder
{
	if (self = [super initWithCoder:coder]) {
		[self registerForDraggedTypes:
			[NSArray arrayWithObject:NSStringPboardType]];
		[self prepareAttributes];
		if ([coder allowsKeyedCoding]) {
			//[self setAngle:[coder decodeObjectForKey:@"angle"]];
			//[self setString:[coder decodeObjectForKey:@"string"]];
			//[self setCopyable:[coder decodeBoolForKey:@"copyable"]];
		} else {
			//[self setAngle:[coder decodeObject]];
			//[self setString:[coder decodeObject]];
			//[coder decodeValueOfObjCType:@encode(BOOL) 
			//						  at:&copyable];
		}
	}
	return self;
}
*/

- (void)drawRect:(NSRect)rect
{

	NSRect bounds = [self bounds];
	
	//resize image
	NSSize imageSize;
	[bgImage setScalesWhenResized:YES];
	imageSize.height=40;
	imageSize.width = 40;
	[bgImage setSize:imageSize];
	
	
	[[NSColor colorWithPatternImage:bgImage] set];
	
	//rotate image
	// Rotate coordinate system abount center of bounds
	NSAffineTransform  *newTransform = [NSAffineTransform transform];
	[newTransform translateXBy:bounds.size.width/2.0 yBy: 
		bounds.size.height/2.0];
	[newTransform rotateByDegrees:-testAngle];
	[newTransform translateXBy:-bounds.size.width/2.0 yBy:- 
		bounds.size.height/2.0];
	[newTransform concat];

	// Draw current image centered in bounds
	NSImage *currentImage = bgImage;
	NSRect  imageRect = NSZeroRect;

	imageRect.size = [currentImage size];

	[currentImage drawAtPoint:NSMakePoint(bounds.size.width/ 
										  2.0-imageRect.size.width/2.0,
										  bounds.size.height/ 
										  2.0-imageRect.size.height/2.0)
							fromRect:imageRect  
							operation:NSCompositeSourceOver fraction:1.0];
		
		//Alt
		//[currentImage drawInRect:bounds fromRect:imageRect operation:NSCompositeSourceOver  fraction:1.0];
	
	// Draw center point just for testing
	//[[NSColor redColor] set];
	//NSRectFill(NSMakeRect(bounds.size.width/2.0 - 5, bounds.size.height/2.0 - 5, 10, 10))  ;	
}


-(void)mouseDragged:(NSEvent *)event
{

	NSPoint clickPoint =  [event locationInWindow];
	currentPoint =  [self convertPoint:clickPoint fromView:nil];
	
	testAngle = currentPoint.y + clickOffset;
	
	if (testAngle >= 1 && testAngle <= 265) {
		[self setNeedsDisplay:YES];
		lastPoint = testAngle;
	}
	
	//NSLog(@"new mouse point = %f", testAngle);
}

-(void)mouseDown:(NSEvent *)event
{
	float newClickPoint;
	
	NSPoint clickPoint =  [event locationInWindow];
	currentPoint =  [self convertPoint:clickPoint fromView:nil];
	newClickPoint = currentPoint.y;

	//calculate offset from last mouse point and current
	clickOffset = lastPoint - newClickPoint;

	NSLog(@"new mouse point = %f", clickOffset);
}


-(IBAction)setAngle:(id)sender

{
	testAngle = [sender floatValue];
	//[self displayAll];
	[self setNeedsDisplay:YES];
	
}

- (float)getKnobLinearValue
{
	return lastPoint/255;
}


- (void)dealloc
{
	
	[circleBase release];
	[circleInner release];
	[settingPath release];
	[super dealloc];
}


@end
