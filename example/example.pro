TARGET = example
TEMPLATE = app
VERSION = 1.0.0

#CONFIG += thismBuildShared

QT += core
QT += widgets
CONFIG += c++11
CONFIG += warn_on
CONFIG += exceptions

DEFINES += DO_SIMULATION
DEFINES += "BAHA_TYPE=BAHA_simu"
DEFINES += "BAHA_INCLUDE=thism/baha_simu.h"


#thismBuildShared {
#	CONFIG += shared
#}
#!thismBuildShared {
#	CONFIG += staticlib
#}

INCLUDEPATH += $$PWD/../libsimu/include
INCLUDEPATH += $$PWD/../lib/include
INCLUDEPATH += include


#LIBS += -L../libsimu -lthism_simu

#DISTFILES += \
#    ../lib/lib.pri


include(../lib/lib.pri)
include(../libsimu/libsimu.pri)
#SOURCES += ../lib/src/baha_simu.cpp


#HEADERS += \
#    include/thism/simuctrl.h
#
#SOURCES += \
#    src/simuctrl.cpp

#FORMS += \
#    $$PWD/../libsimu/simuctrl.ui

SOURCES += \
    src/main.cpp \
    src/smsys.cpp \
    src/worker.cpp

HEADERS += \
    include/smsys.h \
    include/worker.h \
    include/main.h
