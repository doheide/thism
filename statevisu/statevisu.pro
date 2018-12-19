TARGET = statevisu
TEMPLATE = app
VERSION = 1.0.0

QT += core
CONFIG += c++11

DEFINES += DO_SIMULATION
DEFINES += "BAHA_TYPE=BAHA_Void"
DEFINES += "BAHA_INCLUDE=baha_base.h"

DEFINES += __make_treeuml

INCLUDEPATH += \
    $$PWD/../lib/include \
    $$PWD/../example/include


include(../lib/lib.pri)

SOURCES += \
    main.cpp \
    ../example/src/smsys.cpp

