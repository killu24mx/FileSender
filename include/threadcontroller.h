#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H
#include "user.h"
#include <QObject>
#include <QThread>
#include "mainwindow.h"

class ThreadController : public QObject
{
    Q_OBJECT
public:
     ThreadController(MainWindow* win,User* user);
    ~ThreadController();
public slots:

void ask_to_recive_file( QString* file_path,QString file_name,
                         qint64 size,QString userIP);

void message(QString msg);
private:
    QThread user_thread;
    User* user;
    MainWindow* win;

};

#endif // THREADCONTROLLER_H
