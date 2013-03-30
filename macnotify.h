#ifndef NOTIFY_H
#define NOTIFY_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QMap>

#import <objc/runtime.h>

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
	void notificationClicked(int notifyId);
	void removeNotification(int notifyId);
	void showNSUserNotification(const NotificationStrings strings, int ANotifyId);

signals:
	void clicked(int notifyId);

};

#endif // NOTIFY_H
