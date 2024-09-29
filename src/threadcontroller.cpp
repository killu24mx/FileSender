#include "include/threadcontroller.h"


ThreadController::ThreadController(MainWindow* win,User *user): user(user),win(win)
{
    user->moveToThread(&user_thread);
    connect(&user_thread,&QThread::started,user,&User::Listen);
    connect(user,&User::ask_to_recive_file,this,&ThreadController::ask_to_recive_file,
            Qt::BlockingQueuedConnection);
    connect(user,&User::message,this,&ThreadController::message,
             Qt::BlockingQueuedConnection);
    user_thread.start();
}

ThreadController::~ThreadController()
{
    user_thread.quit();
    user_thread.wait();
}

void ThreadController::ask_to_recive_file(QString* file_path,QString file_name,
                             qint64 size,QString userIP)
{
    NewDataMessage msgdialog(*file_path,file_name,size,userIP,win);
    int key = msgdialog.exec();
    if(key != NewDataMessage::Accepted)
        *file_path = "";
}

void ThreadController::message(QString msg)
{
    MessageDialog message(msg,win);
    message.exec();
}
