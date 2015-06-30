#include "macintegration.h"
#include <definitions/notificationdataroles.h>
#include <definitions/optionvalues.h>
#include <utils/options.h>

MacIntegration::MacIntegration()
{
	FOptionsManager = NULL;
	FNotifications = NULL;
}

MacIntegration::~MacIntegration()
{

}

void MacIntegration::pluginInfo(IPluginInfo *APluginInfo)
{
	APluginInfo->name = tr("OS X Integration");
	APluginInfo->description = tr("Provide integration in OS X 10.8 system");
	APluginInfo->version = "0.1";
	APluginInfo->author = "Alexey Ivanov aka krab";
	APluginInfo->homePage = "http://code.google.com/p/vacuum-plugins";
	APluginInfo->dependences.append(NOTIFICATIONS_UUID);
	APluginInfo->dependences.append(OPTIONSMANAGER_UUID);
}

bool MacIntegration::initConnections(IPluginManager *APluginManager, int &AInitOrder)
{
	AInitOrder = 1000;

	IPlugin *plugin = APluginManager->pluginInterface("IOptionsManager").value(0);
	if(plugin)
	{
		FOptionsManager = qobject_cast<IOptionsManager *>(plugin->instance());
	}
	plugin = APluginManager->pluginInterface("INotifications").value(0);
	if(plugin)
	{
		FNotifications = qobject_cast<INotifications *>(plugin->instance());
		connect (FNotifications->instance(), SIGNAL(notificationActivated(int)),this,SLOT(onNotificationRemoved(int)));
		connect (FNotifications->instance(), SIGNAL(notificationRemoved(int)),this,SLOT(onNotificationRemoved(int)));
	}

	return true;
}

bool MacIntegration::initObjects()
{
	FNotifications->insertNotificationHandler(NHO_MACINTEGRATION, this);
	FMacNotify = new MacNotify(this);
	connect(FMacNotify,SIGNAL(clicked(int, QString)),this,SLOT(onNotifyClicked(int, QString)));

	return true;
}

bool MacIntegration::showNotification(int AOrder, ushort AKind, int ANotifyId, const INotification &ANotification)
{
	if (AOrder != NHO_MACINTEGRATION || !(AKind & INotification::PopupWindow))
		return false;

	NotificationData data = {
		ANotification.data.value(NDR_POPUP_CAPTION).toString(),
		ANotification.data.value(NDR_POPUP_TITLE).toString(),
		ANotification.data.value(NDR_POPUP_TEXT).toString(),
		QPixmap::fromImage(qvariant_cast<QImage>(ANotification.data.value(NDR_POPUP_IMAGE)))
	};

	FMacNotify->showNSUserNotification(data, FOptionsManager->currentProfile(), ANotifyId);
	return true;
}

void MacIntegration::onNotifyClicked(int notifyId, QString profile)
{
	if (profile == FOptionsManager->currentProfile())
		FNotifications->activateNotification(notifyId);
}

void MacIntegration::onNotificationRemoved(int notifyId)
{
	FMacNotify->removeNotification(notifyId, FOptionsManager->currentProfile());
}

Q_EXPORT_PLUGIN2(plg_macintegration, MacIntegration)
