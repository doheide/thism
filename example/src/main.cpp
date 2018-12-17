

//#include <thism/simuctrl.h>

//#include <worker.h>
#include <worker.h>

#include <QApplication>
#include <QStringList>
#include <iostream>

#include <main.h>


bool QMyApplication::notify(QObject* receiver, QEvent* event)
{
    try {

        return QApplication::notify(receiver, event);

    } catch (std::exception &e) {
        qFatal("Error %s sending event %s to object %s (%s)",
            e.what(), typeid(*event).name(), qPrintable(receiver->objectName()),
            typeid(*receiver).name());
    } catch (...) {
        qFatal("Error <unknown> sending event %s to object %s (%s)",
            typeid(*event).name(), qPrintable(receiver->objectName()),
            typeid(*receiver).name());
    }

    // qFatal aborts, so this isn't really necessary
    // but you might continue if you use a different logging lib
    return false;
}


SMSys *smsys;


int main(int argc, char *argv[])
{
    QMyApplication app(argc, argv);

    simu_worker::WorkerBase w;
    // Worker w;

    SimuCtrl simu;
    simu.init(&w);

    simu.show();

    return app.exec();
}
