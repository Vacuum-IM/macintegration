#ifndef MACSTRINGHELPER_H
#define MACSTRINGHELPER_H

#import <Foundation/NSString.h>
#include <QString>

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

#endif // MACSTRINGHELPER_H
