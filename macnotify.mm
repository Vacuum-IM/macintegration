#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

#include "macnotify.h"


@interface MacNotificationCenterDelegate : NSObject <NSUserNotificationCenterDelegate> {
	MacNotify* MacNotifyObserver;
}
	- (MacNotificationCenterDelegate*) initialise:(MacNotify*)observer;
	- (void) userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification;
	- (BOOL) userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification;
	- (void) removeOldNotificationWithID:(NSNumber *)uniqueID;
@end

@implementation MacNotificationCenterDelegate
- (MacNotificationCenterDelegate*) initialise:(MacNotify*)observer
{
	if ( (self = [super init]) )
	{
		self->MacNotifyObserver = observer;
	}
	return self;
}

- (void) userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification
{
	Q_UNUSED(center);
	int notifyId = [[[notification userInfo] objectForKey:@"notifyId"] intValue];
	self->MacNotifyObserver->notificationClicked(notifyId);
}

- (BOOL) userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification
{
	Q_UNUSED(center);
	Q_UNUSED(notification);
	return YES;
}

- (void) removeOldNotificationWithID:(NSNumber *)uniqueID
{
	NSUserNotificationCenter *notificationCenter = [NSUserNotificationCenter defaultUserNotificationCenter];
	for (NSUserNotification *deliveredUserNotification in [notificationCenter deliveredNotifications])
	{
		if ([[[deliveredUserNotification userInfo] objectForKey:@"notifyId"] isEqualToNumber:uniqueID])
		{
			[notificationCenter removeDeliveredNotification:deliveredUserNotification];
		}
  }
}

@end

MacNotify::MacNotify(QObject *parent) : QObject(parent)
{
	MacNotificationCenterDelegate* macDelegate = [[MacNotificationCenterDelegate alloc] initialise: this];
	MacNotifyWrapped = macDelegate;
	[[NSUserNotificationCenter defaultUserNotificationCenter] setDelegate:macDelegate];
	[[NSUserNotificationCenter defaultUserNotificationCenter] removeAllDeliveredNotifications];
}

MacNotify::~MacNotify()
{
	[[NSUserNotificationCenter defaultUserNotificationCenter] removeAllDeliveredNotifications];
}

void MacNotify::showNSUserNotification(const NotificationStrings strings, int ANotifyId)
{
	NSUserNotification *userNotification = [[NSUserNotification alloc] init];
	userNotification.title = [NSString stringWithCharacters:(const unichar *)strings.title.unicode() length:(NSUInteger)strings.title.length()];
	userNotification.subtitle = [NSString stringWithCharacters:(const unichar *)strings.subtitle.unicode() length:(NSUInteger)strings.subtitle.length()];
	userNotification.informativeText = [NSString stringWithCharacters:(const unichar *)strings.message.unicode() length:(NSUInteger)strings.message.length()];
	NSDictionary *userInfo = [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithInt:ANotifyId], @"notifyId", nil];
	[userNotification setUserInfo:userInfo];

	[[NSUserNotificationCenter defaultUserNotificationCenter] deliverNotification:userNotification];
}

void MacNotify::notificationClicked(int notifyId)
{
	emit clicked(notifyId);
}

void MacNotify::removeNotification(int notifyId)
{
	[MacNotifyWrapped removeOldNotificationWithID:[NSNumber numberWithInt:notifyId]];
}

