
#include "thism/baha_simu.h"
#include <QString>
#include <QTimerEvent>


void BAHA_simu_log_impl::emitLogEvent(QString s, bool b) {
    emit logEvent(s, b);
}

void BAHA_simu::log(const char *c) {
    logImpl->emitLogEvent(QString(c), false);
}

void BAHA_simu::logLineEnd() {
    logImpl->emitLogEvent(QString(), true);
}

BAHA_simu::BAHA_simu() {
    this->tickImpl = new BAHA_tick_impl(this);
    this->logImpl = new BAHA_simu_log_impl();
}

BAHA_simu::~BAHA_simu() {
    this->tickImpl->deleteLater();
    this->logImpl->deleteLater();
}

