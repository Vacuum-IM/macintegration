#ifndef MACNOTIFY_H
#define MACNOTIFY_H

#import <objc/runtime.h>

#include <QObject>
#include <QString>
#include <QPixmap>

#ifdef __OBJC__
@class MacNotificationCenterDelegate;
#else
typedef struct objc_object MacNotificationCenterDelegate;
#endif

struct NotificationData {
	QString title;
	QString subtitle;
	QString message;
	QPixmap pixmap;
};

class MacNotify : public QObject
{
	Q_OBJECT
public:
	explicit MacNotify(QObject *parent = 0);
	~MacNotify();

	MacNotificationCenterDelegate *MacNotificationCenterWrapped;

public:
	void notificationClicked(int notifyId, QString profile);
	void removeNotification(int notifyId, QString profile);
	void showNSUserNotification(const NotificationData data, QString profile, int ANotifyId);

signals:
	void clicked(int notifyId, QString profile);

};

#endif // MACNOTIFY_H
