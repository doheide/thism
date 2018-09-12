#ifndef HAWAL_QT_H
#define HAWAL_QT_H

#include "sm2.h"


class HAWAL_qt : public HAWAL_Base
{
public:
    HAWAL_qt();

    virtual void log(const char *c);
    virtual void logLineEnd();

};

#endif // HAWAL_QT_H
