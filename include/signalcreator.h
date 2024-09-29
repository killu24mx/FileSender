#ifndef SIGNAINTERCEPTOR_H
#define SIGNAINTERCEPTOR_H

#include <QObject>
#include "user.h"

class SignalCreator : public QObject
{
    Q_OBJECT
public:
    SignalCreator(const SignalCreator&) = delete;
    SignalCreator& operator=(const SignalCreator&) = delete;

    static bool CreateSignalLinks(MainWindow *main_win,User* user);
signals:


private:
    SignalCreator(){}
    ~SignalCreator(){}
};

#endif // SIGNAINTERCEPTOR_H
