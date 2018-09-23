//
// MIT License
//
// Copyright (c) 2018 Dominik Heide
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef HAWAL_QT_H
#define HAWAL_QT_H

#include <thism/sm2.h>

#include <QObject>
#include <QBasicTimer>
#include <QTimerEvent>
#include <iostream>


class BAHA_qt;

class BAHA_qt_impl : public QObject {
    Q_OBJECT

    BAHA_qt *parent;
    QBasicTimer sysTick_timer;
    QBasicTimer eventloop_timer;

public:
    BAHA_qt_impl(BAHA_qt *p) : parent(p) {
        sysTick_timer.start(100, this);
        eventloop_timer.start(1, this);
    }

    void timerEvent(QTimerEvent *event);
};

class BAHA_qt : public BAHA_Base
{
    BAHA_qt_impl *qt_impl;

public:
    BAHA_qt();

    virtual void log(const char *c);
    virtual void logLineEnd();

};

#endif // HAWAL_QT_H
