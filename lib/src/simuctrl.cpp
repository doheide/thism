#include <thism/simuctrl.h>

#ifdef DO_SIMULATION

#include "ui_simuctrl.h"
#include <QRegularExpression>


extern SMSys *smsys;

simu_worker::Worker::Worker() : smsys_inst(&baha) {
    this->isPause = true;

    connect(this, SIGNAL(paused()), this, SLOT(pause()));

    // *********************************
    smsys = &smsys_inst;

    smsys_inst.setSimuCallbacks(this);
}

void simu_worker::Worker::doWork() {
    while(true) {
        if(!this->isPause)
            smsys_inst.processEvents();
        QApplication::processEvents();
    }
}

void simu_worker::Worker::pause() {
    this->isPause = true;
    this->baha.getSimuImpl()->setBlockTimer(true);
}

void simu_worker::Worker::run() {
    this->isPause = false;
    this->baha.getSimuImpl()->setBlockTimer(false);
}

SimuCtrl::SimuCtrl(QWidget *parent) : QWidget(parent), ui(new Ui::SimuCtrl) {

    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 2);

    // ****************************************************
    thread = new QThread(this);
    worker = new simu_worker::Worker();
    worker->moveToThread(thread);

    connect(this, SIGNAL(run()), worker, SLOT(run()));
    connect(this, SIGNAL(pause()), worker, SLOT(pause()));

    connect(thread, SIGNAL(started()), worker, SLOT(doWork()));

    connect(this, SIGNAL(setPauseOnEvents(QStringList)), worker, SLOT(setPauseOnEvents(QStringList)));
    connect(this, SIGNAL(setPauseOnEnterStates(QStringList)), worker, SLOT(setPauseOnEnterStates(QStringList)));
    connect(this, SIGNAL(setPauseOnExitStates(QStringList)), worker, SLOT(setPauseOnExitStates(QStringList)));

    connect(this, SIGNAL(pause()), this, SLOT(setupPaused()));
    connect(worker, SIGNAL(paused()), this, SLOT(setupPaused()));

    connect(worker->getBaha()->getTickImpl(), SIGNAL(logEvent(QString, bool)), this, SLOT(logEvent(QString, bool)));

    this->thread->start();

    // *************************************************
    this->running = false;
    {
        QStringList sl;
        for(int i=0; i!=SMSys::EventListT::AllEvents::size; i++)
            sl.append(event_details::getEventName<SMSys::EventListT>(i));

        this->addEvents(sl);
    }
    {
        QStringList sl;
        for(int i=0; i!=SMSys::numberOfStatesT::value; i++)
            sl.append(smsys->getStateById(i)->name());

        this->addStates(sl);
    }

}

SimuCtrl::~SimuCtrl() {
    worker->deleteLater();
    thread->deleteLater();

    delete ui;
}

void SimuCtrl::addEvents(QStringList sl) {
    ui->lw_event->addItems(sl);
}

void SimuCtrl::addStates(QStringList sl) {
    ui->lw_onEnter->addItems(sl);
    ui->lw_onExit->addItems(sl);
}

void SimuCtrl::setupPaused() {
    ui->pb_run->setText("run");
    this->running = false;
}

void SimuCtrl::on_pb_run_clicked() {
    if(this->running) {
        // is running -> set to pause
        emit pause();
    }
    else {
        // is not running -> set to pause
        ui->pb_run->setText("pause");
        emit run();

        this->running = true;
    }
}


void SimuCtrl::on_lw_onEnter_itemSelectionChanged() {
    this->pauseOnEnterState.clear();
    foreach (QListWidgetItem *cwi, ui->lw_onEnter->selectedItems()) {
        this->pauseOnEnterState.append(cwi->text());
    }
    emit setPauseOnEvents(this->pauseOnEnterState);
}

void SimuCtrl::on_lw_onExit_itemSelectionChanged() {
    this->pauseOnExitState.clear();
    foreach (QListWidgetItem *cwi, ui->lw_onExit->selectedItems()) {
        this->pauseOnExitState.append(cwi->text());
    }
    emit setPauseOnEnterStates(this->pauseOnExitState);
}

void SimuCtrl::on_lw_event_itemSelectionChanged() {
    this->pauseOnEvents.clear();
    foreach (QListWidgetItem *cwi, ui->lw_event->selectedItems()) {
        this->pauseOnEvents.append(cwi->text());
    }
    emit setPauseOnExitStates(this->pauseOnEvents);
}

void SimuCtrl::logEvent(QString log, bool lineEnd) {
    if(!lineEnd) {
        logLine += log;
    }
    else {
        ui->pte_log->appendPlainText(logLine);

        if(ui->gb_pauseOnState->isChecked()) {
            // check if is state enter
           auto reSEn = QRegularExpression ("^[0-9]* \\| !! EN ([a-zA-Z0-9_]*)$");
            auto match = reSEn.match(logLine);
            if (match.hasMatch()) {
                QString m = match.captured(1);
                if(this->pauseOnEnterState.contains(m)) {
                    this->ui->pte_log->appendPlainText(
                                QString("<Pause onEnter: %1>").arg(m));
                    emit pause();
                }
            }

            // check if is state exit
            reSEn = QRegularExpression ("^[0-9]* \\| !! EX ([a-zA-Z0-9_]*)$");
            match = reSEn.match(logLine);
            if (match.hasMatch()) {
                QString m = match.captured(1);
                if(this->pauseOnExitState.contains(m)) {
                    this->ui->pte_log->appendPlainText(
                                QString("<Pause onExit: %1>").arg(m));
                    emit pause();
                }
            }
        }

        if(ui->gb_pauseOnEvent->isChecked() ) {
            // check if event
            auto reSEn = QRegularExpression ("^[0-9]* \\| !! R ([a-zA-Z0-9_]*) \\| [a-zA-Z0-9_]*$");
            auto match = reSEn.match(logLine);
            if (match.hasMatch()) {
                if(ui->cb_nextEvent->isChecked()) {
                    this->ui->pte_log->appendPlainText(QString("<Pause any onEvent>"));
                    emit pause();
                } else {
                    QString m = match.captured(1);
                    if(this->pauseOnEvents.contains(m)) {
                        this->ui->pte_log->appendPlainText(QString("<Pause onEvent: %1>").arg(m));
                        emit pause();
                    }
                }
            }
        }
        logLine = "";
    }
}


#endif

