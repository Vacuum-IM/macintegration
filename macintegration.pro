include(qmake/debug.inc)
include(qmake/config.inc)

#Project configuration
TARGET = macintegration
QT = core gui xml
QMAKE_LFLAGS    += -framework Foundation -framework Cocoa
include(macintegration.pri)

#Default progect configuration
include(qmake/plugin.inc)

#Translation
TRANS_SOURCE_ROOT = .
include(translations/languages.inc)
