#ifndef NOTIFY_H
#define NOTIFY_H

#import <objc/runtime.h>

#include <QObject>
#include <QString>
#include <QDebug>

#ifdef __OBJC__
@class MacNotificationCenterDelegate;
#else
typedef struct objc_object MacNotificationCenterDelegate;
#endif

struct NotificationStrings {
	QString title;
	QString subtitle;
	QString message;
};

class MacNotify : public QObject
{
	Q_OBJECT
public:
	explicit MacNotify(QObject *parent = 0);
	~MacNotify();

	MacNotificationCenterDelegate *MacNotifyWrapped;

public:
	void notificationClicked(int notifyId, QString profile);
	void removeNotification(int notifyId, QString profile);
	void showNSUserNotification(const NotificationStrings strings, QString profile, int ANotifyId);

signals:
	void clicked(int notifyId, QString profile);

};

#endif // NOTIFY_H
