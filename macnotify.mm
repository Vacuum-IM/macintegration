#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

#include "macstringhelper.h"
#include "macnotify.h"

@interface MacNotificationCenterDelegate : NSObject <NSUserNotificationCenterDelegate> {
	MacNotify* MacNotifyObserver;
}
	- (MacNotificationCenterDelegate*) initialise:(MacNotify*)observer;
	- (void) userNotificationCenter:(NSUserNotificationCenter *)center didActivateNotification:(NSUserNotification *)notification;
	- (BOOL) userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification;
	- (void) removeOldNotificationWithID: (NSNumber *)uniqueID andProfile:(NSString *)profile;
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
	QString profile = toQString([[notification userInfo] objectForKey:@"profile"]);
	self->MacNotifyObserver->notificationClicked(notifyId, profile);
}

- (BOOL) userNotificationCenter:(NSUserNotificationCenter *)center shouldPresentNotification:(NSUserNotification *)notification
{
	Q_UNUSED(center);
	Q_UNUSED(notification);
	return YES;
}

- (void) removeOldNotificationWithID:(NSNumber *)uniqueID andProfile:(NSString *)profile
{
	NSUserNotificationCenter *notificationCenter = [NSUserNotificationCenter defaultUserNotificationCenter];
	for (NSUserNotification *deliveredUserNotification in [notificationCenter deliveredNotifications])
	{
		if ([[[deliveredUserNotification userInfo] objectForKey:@"profile"] isEqualToString:profile])
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

void MacNotify::showNSUserNotification(const NotificationStrings strings, QString profile, int ANotifyId)
{
	NSUserNotification *userNotification = [[NSUserNotification alloc] init];
	userNotification.title = toNSString(strings.title);
	userNotification.subtitle = toNSString(strings.subtitle);
	userNotification.informativeText = toNSString(strings.message);
	NSDictionary *userInfo = [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithInt:ANotifyId], @"notifyId", toNSString(profile), @"profile", nil];
	[userNotification setUserInfo:userInfo];

	[[NSUserNotificationCenter defaultUserNotificationCenter] deliverNotification:userNotification];
}

void MacNotify::notificationClicked(int notifyId, QString profile)
{
	emit clicked(notifyId, profile);
}

void MacNotify::removeNotification(int notifyId, QString profile)
{
	[MacNotifyWrapped removeOldNotificationWithID:[NSNumber numberWithInt:notifyId] andProfile:toNSString(profile)];
}

