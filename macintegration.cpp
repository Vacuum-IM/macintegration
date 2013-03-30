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
		connect (FNotifications->instance(), SIGNAL(notificationActivated(int)),this,SLOT(notifyAboutToRemove(int)));
		connect (FNotifications->instance(), SIGNAL(notificationRemoved(int)),this,SLOT(notifyAboutToRemove(int)));
	}

	return true;
}

bool MacIntegration::initObjects()
{
	FNotifications->insertNotificationHandler(NHO_MACINTEGRATION, this);
	FMacNotify = new MacNotify(this);
	connect(FMacNotify,SIGNAL(clicked(int)),this,SLOT(notifyClicked(int)));

	return true;
}

bool MacIntegration::showNotification(int AOrder, ushort AKind, int ANotifyId, const INotification &ANotification)
{
	if (AOrder != NHO_MACINTEGRATION || !(AKind & INotification::PopupWindow))
		return false;

	NotificationStrings strings = {
		ANotification.data.value(NDR_POPUP_CAPTION).toString(),
		ANotification.data.value(NDR_POPUP_TITLE).toString(),
		filter(ANotification.data.value(NDR_POPUP_HTML).toString()),
	};


	FMacNotify->showNSUserNotification(strings, ANotifyId);
	return true;
}

void MacIntegration::notifyClicked(int notifyId)
{
	FNotifications->activateNotification(notifyId);
}

void MacIntegration::notifyAboutToRemove(int notifyId)
{
	FMacNotify->removeNotification(notifyId);
}

QString MacIntegration::filter(const QString &text)
{
	QString out = text;
	out.replace(QLatin1Char('\n'), QString());
	out.replace(QLatin1String("</p>"), QLatin1String("\n"), Qt::CaseInsensitive);
	out.replace(QLatin1String("<br />"), QLatin1String("\n"), Qt::CaseInsensitive);

	int lt = 0;
	int gt = 0;
	forever
	{
		lt = out.indexOf(QLatin1Char('<'), lt);
		if (lt == -1)
			break;
		gt = out.indexOf(QLatin1Char('>'), lt);
		if (gt == -1)
		{
			out.remove(lt, out.size() - lt);
			break;
		}
		out.remove(lt, gt - lt + 1);
	}

  out.replace(QLatin1String("&gt;"), QLatin1String(">"));
  out.replace(QLatin1String("&lt;"), QLatin1String("<"));
  out.replace(QLatin1String("&quot;"), QLatin1String("\""));
  out.replace(QLatin1String("&nbsp;"), QLatin1String(" "));
  out.replace(QLatin1String("&amp;"), QLatin1String("&"));
  out.replace(QChar(QChar::Nbsp), QLatin1String(" "));
  out = out.trimmed();
  return out;
}

Q_EXPORT_PLUGIN2(plg_macintegration, MacIntegration)
