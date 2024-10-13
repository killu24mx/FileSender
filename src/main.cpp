#include "include/signalcreator.h"
#include "include/loaddataprogressdialog.h"
#include <QApplication>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QInputDialog>

#include "include/threadcontroller.h"
QHostAddress getLocalIP()
{
   QHostAddress IP;
   const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);

   for (const QHostAddress &address: QNetworkInterface::allAddresses())
       if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
             IP = address;

   return IP;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow win;

    User user(getLocalIP());

    SignalCreator::CreateSignalLinks(&win,&user);
    ThreadController thc(&win,&user);

    win.show();

    return app.exec();
}
