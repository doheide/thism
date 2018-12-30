#ifndef BAHA_SIMU_H
#define BAHA_SIMU_H

#include <QObject>
#include <QString>

#include <thism/baha_qt.h>


class BAHA_simu_log_impl : public QObject {
    Q_OBJECT
signals:
   void logEvent(QString, bool);

public:
   void emitLogEvent(QString, bool);
};

class BAHA_simu : public BAHA_Base
{
    BAHA_tick_impl *tickImpl;
    BAHA_simu_log_impl *logImpl;

public:
    BAHA_simu();
    virtual ~BAHA_simu();

    virtual void log(const char *c);
    virtual void logLineEnd();

    BAHA_tick_impl *getTickImpl() {
        return tickImpl;
    }
    BAHA_simu_log_impl *getLogImpl() {
        return logImpl;
    }
};


#endif // BAHA_SIMU_H
