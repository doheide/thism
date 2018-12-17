#ifndef MAIN_H
#define MAIN_H

#include <QObject>
#include <QApplication>


class QMyApplication Q_DECL_FINAL : public QApplication
{
    Q_OBJECT
public:
    QMyApplication(int &argc, char **argv) : QApplication(argc, argv) {}

    bool notify(QObject* receiver, QEvent* event) Q_DECL_OVERRIDE;
};


#endif // MAIN_H
