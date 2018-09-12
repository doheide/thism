#include "hawal_qt.h"

HAWAL_qt::HAWAL_qt()
{

}

void HAWAL_qt::log(const char *c) {
    std::cout << c;
}

void HAWAL_qt::logLineEnd() {
    std::cout << std::endl;
}
