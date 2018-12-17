#ifndef BAHA_SIMU_H
#define BAHA_SIMU_H

#include <QObject>
#include <QBasicTimer>

#include <thism/baha_base.h>

class BAHA_simu;

class BAHA_simu_impl : public QObject {
    Q_OBJECT

    BAHA_simu *parent;
    QBasicTimer sysTick_timer;
//    QBasicTimer eventloop_timer;
    bool blockTimer;

signals:
    void logEvent(QString, bool);

public:
    BAHA_simu_impl(BAHA_simu *p) : parent(p), blockTimer(true) {
        sysTick_timer.start(100, this);
//        eventloop_timer.start(1, this);
    }

    void timerEvent(QTimerEvent *event);

    void setBlockTimer(bool v)
    { blockTimer = v; }
};

class BAHA_simu : public BAHA_Base
{
    BAHA_simu_impl *simuImpl;

public:
    BAHA_simu();
    ~BAHA_simu();

    virtual void log(const char *c);
    virtual void logLineEnd();

    BAHA_simu_impl *getSimuImpl() {
        return simuImpl;
    }
};

#include <thism/sm2.h>


#endif // BAHA_SIMU_H
