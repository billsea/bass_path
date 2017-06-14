/*
*	File:		CocoaViewFactory.h
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
#import <AudioUnit/AUCocoaUIView.h>
#import <audioControls/audioControls.h>

//#error Rename ALL UI classes with a unique name to avoid namespace collisions
/************************************************************************************************************/
/* NOTE: It is important to rename ALL ui classes when using the XCode Audio
 * Unit with Cocoa View template	*/
/*		 Cocoa has a flat namespace, and if you use the default filenames,
 * it is possible that you will		*/
/*		 get a namespace collision with classes from the cocoa view of a
 * previously loaded audio unit.		*/
/*		 We recommend that you use a unique prefix that includes the
 * manufacturer name and unit name on		*/
/*		 all objective-C source files. You may use an underscore in your
 * name, but please refrain from		*/
/*		 starting your class name with an undescore as these names are
 * reserved for Apple.					*/
/*  Example  : AppleDemoFilter_UIView AppleDemoFilter_ViewFactory
 */
/************************************************************************************************************/

@class bassAmp_CocoaView;

@interface bassAmp_CocoaViewFactory : NSObject <AUCocoaUIBase> {
  bassAmp_CocoaView *uiFreshlyLoadedView;
}

- (NSString *)description; // string description of the view

@end
