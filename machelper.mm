#include "machelper.h"

CGImageRef toCGImageRef(const QPixmap &pixmap)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	QPlatformNativeInterface::NativeResourceForIntegrationFunction function = resolvePlatformFunction("qimagetocgimage");
	if (function) {
		typedef CGImageRef (*QImageToCGImageFunction)(const QImage &image);
		return reinterpret_cast<QImageToCGImageFunction>(function)(pixmap.toImage());
	}

	return NULL;
#else
	return pixmap.toMacCGImageRef();
#endif
}

QPixmap fromCGImageRef(CGImageRef image)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	QPlatformNativeInterface::NativeResourceForIntegrationFunction function = resolvePlatformFunction("cgimagetoqimage");
	if (function) {
		typedef QImage (*CGImageToQImageFunction)(CGImageRef image);
		return QPixmap::fromImage(reinterpret_cast<CGImageToQImageFunction>(function)(image));
	}

	return QPixmap();
#else
	return QPixmap::fromMacCGImageRef(image);
#endif
}

NSImage* toNSImage(const QPixmap &pixmap)
{
	if (pixmap.isNull())
		return 0;
	CGImageRef cgimage = toCGImageRef(pixmap);
	NSBitmapImageRep *bitmapRep = [[NSBitmapImageRep alloc] initWithCGImage:cgimage];
	NSImage *image = [[NSImage alloc] init];
	[image addRepresentation:bitmapRep];
	[bitmapRep release];
	CFRelease(cgimage);
	return image;
}

