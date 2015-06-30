#ifndef MACHELPER_H
#define MACHELPER_H

#import <Foundation/Foundation.h>
#include <QString>
#include <QPixmap>


#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtCore/QDebug>
#include <QtGui/QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

inline QPlatformNativeInterface::NativeResourceForIntegrationFunction resolvePlatformFunction(const QByteArray &functionName)
{
	QPlatformNativeInterface *nativeInterface = QGuiApplication::platformNativeInterface();
	QPlatformNativeInterface::NativeResourceForIntegrationFunction function =
		nativeInterface->nativeResourceFunctionForIntegration(functionName);
	if (!function)
		 qWarning() << "Qt could not resolve function" << functionName
					<< "from QGuiApplication::platformNativeInterface()->nativeResourceFunctionForIntegration()";
	return function;
}
#endif

static inline NSString* toNSString(const QString &string)
{
	return [NSString stringWithCharacters:(const unichar *)string.unicode() length:(NSUInteger)string.length()];
}

static inline QString toQString(NSString *string)
{
	if (!string)
		return QString();
	return QString::fromUtf8([string UTF8String]);
}

NSImage* toNSImage(const QPixmap &pixmap);

#endif // MACHELPER_H
