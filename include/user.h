#ifndef USER_H
#define USER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "messagedialog.h"

#include "mainwindow.h"
#include "filesender.h"
#include "newdatamessage.h"
#include "loaddataprogressdialog.h"



class User : public QTcpServer
{
Q_OBJECT
public:
    User(QHostAddress IP,quint16 server_port = 0);
    User(QTcpSocket* socket);
    ~User();

    friend class SignalCreator;
    void Listen();

    struct UserInfo{
        QHostAddress IP;
        size_t Id;
    };

    UserInfo getInfo(){return info;}
    void ConnectTo(quint16 port, QHostAddress IP);

    bool isConnected(quint16 port, QHostAddress IP);
    bool CheckSelfConnection(quint16 port, QHostAddress IP);

    void SendFileToUser(User* user,QString file_path);
    User* getUser(qintptr sockDS);

    quint16 get_server_port(){return server_port;}

    static size_t getId(quint16 port,QHostAddress IP);
    User* getUserFromId(size_t id);



signals:
    void ConnectionRes(QString status,bool res);

    void ask_to_recive_file(QString* file_path,QString file_name,
                            qint64 size,QString userIP);

    void message(QString);
    void UserConnected(QString IP,QString id);
    void UserDisconnected(QString id);
    void stop();
    void setInfoLables(const QString& ip,quint16 port);


public slots:
     void newConnectionSlot();
     void ReadyReadSlot();
     void FileDataArrived();
     void ConnectSlot(QString port,QString IP);
     void NewFileSendSlot(QString path,QString id);
     void UserDisconnectedSlot(QString id);

     void stoped();
private:
    UserInfo info;
    struct{
        QTcpSocket* info_socket = nullptr;
        QTcpSocket* data_socket = nullptr;
    }sockets;

    QList<User*> connected_users;
    FileSender file_sender;
    quint16 server_port;

    bool ReadyReadSlotLock = false;
    bool FileArrivedSlotLock = false;

    bool isSender = false;
    bool was_stoped = false;

    User* selected_user;

    enum MessageType{
        FILE_INFO_MESSAGE = 0,
        STOP_TRANSFERING_MESSAGE = 1,
        CONNECTION_MESSAGE = 2,
        ANSWER_BOOL_MESSAGE = 3,
    };

    const quint8 COUNT_OF_MESSAGE_TYPES = 4;

    void SendMessageToUser(User* user, MessageType msg_type, QByteArray msg_data);
    MessageType ReciveMessage(QTcpSocket* socket);

    void ProcessFileInfoMessage(User* user);

    template<typename T>
    void SendMessageToUser_T(User* user, MessageType msg_type, T msg_data){
        SendMessageToUser(user,msg_type,QByteArray(reinterpret_cast<char*>(&msg_data),sizeof(T)));
    }
};
#endif // USER_H

