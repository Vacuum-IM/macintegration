#ifndef MACINTEGRATION_H
#define MACINTEGRATION_H

#include "definitions.h"
#include "macnotify.h"

#include <interfaces/ipluginmanager.h>
#include <interfaces/inotifications.h>
#include <interfaces/ioptionsmanager.h>

class MacIntegration :
        public QObject,
        public IPlugin,
        public INotificationHandler
{
	Q_OBJECT;
    Q_INTERFACES(IPlugin INotificationHandler);
public:
	MacIntegration();
	~MacIntegration();
	//IPlugin
	virtual QObject *instance() { return this; }
	virtual QUuid pluginUuid() const { return MACINTEGRATION_UUID; }
	virtual void pluginInfo(IPluginInfo *APluginInfo);
	virtual bool initConnections(IPluginManager *APluginManager, int &AInitOrder);
	virtual bool initObjects();
	virtual bool initSettings() { return true; }
	virtual bool startPlugin() { return true; }

public:
	QPointer<MacNotify> FMacNotify;
	//INotificationHandler
	virtual bool showNotification(int AOrder, ushort AKind, int ANotifyId, const INotification &ANotification);

protected:
	QString filter(const QString &text);

protected slots:
	void onNotifyClicked(int notifyId, QString profile);
	void onNotificationRemoved(int notifyId);

private:
	IOptionsManager *FOptionsManager;
	INotifications *FNotifications;

};

#endif // MACINTEGRATION_H
