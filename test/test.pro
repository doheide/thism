GOOGLETEST_DIR = ../googletest

include(gtest_dependency.pri)


QT += core

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
#CONFIG -= qt

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage

#DEFINES +=

INCLUDEPATH +=  ../lib/include
DEPENDPATH += $$PWD/../lib/include

include(../lib/lib.pri)

LIBS += -L/usr/lib -lgcov

HEADERS += \
    base_sys/base_test_sys.h \
    test_smsys_compiletime.h \
    test_smsys_sys.h \
    base_sys/base_test_sys_sm.h

SOURCES += \
    main.cpp \
    base_sys/base_test_sys.cpp

RESOURCES += \
    ctt.qrc

