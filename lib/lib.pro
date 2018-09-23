TARGET = thism
TEMPLATE = lib
VERSION = 1.0.0

CONFIG += thismBuildShared

QT += core 

thismBuildShared {
	CONFIG += shared
}
!thismBuildShared {
	CONFIG += staticlib
}

INCLUDEPATH += $$PWD/include


DISTFILES += \
    lib.pri

include(lib.pri)
