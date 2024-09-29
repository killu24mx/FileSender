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

    win.show();
    quint16 port  = (quint16)QInputDialog::getInt(&win, QString::fromUtf8("Input port"),
                                                QString::fromUtf8("Port:"));

    win.setWindowTitle(QString::number(port));
    qDebug() << port;
    qDebug() << getLocalIP();
    User user(getLocalIP(),port);

    SignalCreator::CreateSignalLinks(&win,&user);
    ThreadController thc(&win,&user);



    return app.exec();
}
