#ifndef SIMUCTRL_H
#define SIMUCTRL_H

#ifdef DO_SIMULATION

#include <QObject>
#include <QWidget>
#include <QThread>
#include <QStringList>
#include <QListWidgetItem>

#include <thism/sm2.h>
//#include "sm2_sil.h"

#include "baha_simu.h"
#include "smsys.h"



namespace simu_worker {

    struct Worker : public QObject, Simu_helper::SimuCallbacks {
        Q_OBJECT

        bool isPause;

        // ***************
        BAHA_simu baha;
        SMSys smsys_inst;

    public:
        Worker();

        QStringList pauseOnEvents;
        QStringList pauseOnEnterState;
        QStringList pauseOnExitState;

        BAHA_simu *getBaha() {
            return &baha;
        }

    signals:
        void paused();

    public slots:
        void doWork();

        void pause();
        void run();

        void setPauseOnEvents(QStringList _pauseOnEvents) {
            this->pauseOnEvents = _pauseOnEvents;
        }
        void setPauseOnEnterStates(QStringList _pauseOnEnterState) {
            this->pauseOnEnterState = _pauseOnEnterState;
        }
        void setPauseOnExitStates(QStringList _pauseOnExitState) {
            this->pauseOnExitState = _pauseOnExitState;
        }

    public:
        void onActivateState(uint16_t stateId) {

        }
        void onDeactivateState(uint16_t stateId) {

        }
        void onRaiseEvent(uint16_t eventId) {
            if(this->pauseOnExitState.contains(event_details::getEventName<SMSys::EventListT>(eventId)))
                emit paused();
            QThread::msleep(10);
        }
    };
}



namespace Ui {
class SimuCtrl;
}



class SimuCtrl : public QWidget
{
    Q_OBJECT

    bool running;

    QThread *thread;
    simu_worker::Worker *worker;

    QString logLine;

    QStringList pauseOnEnterState, pauseOnExitState;
    QStringList pauseOnEvents;

signals:
    void pause();
    void run();
    void setPauseOnEvents(QStringList _pauseOnEvents);
    void setPauseOnEnterStates(QStringList _pauseOnEnterState);
    void setPauseOnExitStates(QStringList _pauseOnExitState);

public:
    explicit SimuCtrl(QWidget *parent = 0);
    ~SimuCtrl();

    void addEvents(QStringList sl);
    void addStates(QStringList sl);

public slots:
    void setupPaused();
    void on_pb_run_clicked();
    void on_lw_onEnter_itemSelectionChanged();
    void on_lw_onExit_itemSelectionChanged();
    void on_lw_event_itemSelectionChanged();

    void logEvent(QString log, bool lineEnd);

private:
    Ui::SimuCtrl *ui;
};

#endif

#endif // SIMUCTRL_H
