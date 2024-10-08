#include "include/user.h"


void Error(QString msg){
   qDebug() << "ERROR: " << msg.toUpper();
}

User::User(QTcpSocket *_socket):server_port(0),selected_user(nullptr)
{
    if(_socket){
         this->info.IP =  QHostAddress(_socket->localAddress().toIPv4Address());
         sockets.info_socket = _socket;
    }

    connect(this,&QTcpServer::newConnection,this,&User::newConnectionSlot);

    connect(this,&User::UserDisconnected,this,&User::UserDisconnectedSlot);
}

User::User(QHostAddress IP,quint16 server_port): User(nullptr)
{
     info.IP = IP;
     info.Id = 0;
     this->server_port = server_port;
}

User::~User()
{
    while(!connected_users.isEmpty()){
        delete *connected_users.begin();
        connected_users.erase(connected_users.begin());
    }
}

void User::Listen()
{
    if(listen(QHostAddress::Any,server_port)){
        qDebug() << "server listen";
        server_port = serverPort();
        qDebug() << server_port;
        this->info.Id = User::getId(server_port,this->info.IP);
        emit setInfoLables(this->info.IP.toString(),server_port);

    }
    else
        qDebug() << "error";
}

void User::ConnectTo(quint16 port, QHostAddress IP)
{
    if(isConnected(port,IP)){
        emit ConnectionRes("You are already connected!",false);
        return;
    }
    if(CheckSelfConnection(port,IP)){
        emit  ConnectionRes("You can't connect to yourself!",false);
        return;
    }

    User* user = nullptr;
    bool connection_res = false;

    QTcpSocket* new_user_info_socket = new QTcpSocket();
    new_user_info_socket->connectToHost(IP,port);
    new_user_info_socket->waitForConnected(TIME_OUT);

    if(new_user_info_socket->state() == QTcpSocket::ConnectedState){
        user = new User(new_user_info_socket);
        user->info.Id = User::getId(port,IP);

        connect(user->sockets.info_socket,&QTcpSocket::readyRead,this,&User::ReadyReadSlot);
        connect(user->sockets.info_socket,&QTcpSocket::disconnected,
                user->sockets.info_socket,[user,this](){
            emit UserDisconnected(QString::number(user->info.Id));
        });

        SendMessageToUser_T(user,MessageType::CONNECTION_MESSAGE,this->info.Id);
        qDebug() << this->info.Id << "\tsend";
        connection_res = true;
    }
    else{
        delete new_user_info_socket;
    }

    if(connection_res){
        QTcpSocket* new_user_data_socket = new QTcpSocket();
        new_user_data_socket->connectToHost(IP,port);
        new_user_data_socket->waitForConnected(TIME_OUT);

        if(new_user_data_socket->state() == QTcpSocket::ConnectedState){
            user->sockets.data_socket = new_user_data_socket;
            connect(user->sockets.data_socket,&QTcpSocket::readyRead,this,&User::FileDataArrived);
        }
        else{
             connection_res = false;
             delete new_user_data_socket;
        }
    }

    if(connection_res){
        connected_users.push_back(user);
        qDebug() << "connected to" << user->info.IP << "\t" << user->info.Id;
        emit ConnectionRes("Сonnected to " + IP.toString(), true);
        emit UserConnected(user->info.IP.toString(),QString::number(user->info.Id));
    }

    else{
        qDebug() << "error connection to " << IP.toString();
        if(user) delete user;
        emit ConnectionRes("Error connection!",false);
    }
}


inline bool User::isConnected(quint16 port, QHostAddress IP)
{
    size_t id = User::getId(port,IP);
    return std::find_if(connected_users.begin(),connected_users.end(),[id](const User* user){
        return user->info.Id == id;
    }) != connected_users.end();
}

bool User::CheckSelfConnection(quint16 port, QHostAddress IP)
{
    return info.Id == getId(port,IP);
}

void User::SendFileToUser(User *user, QString file_path)
{
    QTcpSocket* user_info_socket = user->sockets.info_socket;
    QTcpSocket* user_data_socket = user->sockets.data_socket;

    if(!file_sender.Open(file_path,QFile::ReadOnly,user_data_socket))
        Error("Cant open file: " + file_path);

    SendMessageToUser_T(user,MessageType::FILE_INFO_MESSAGE,file_sender.getFileInfo());
    bool answer = recive_unique<bool>(user_info_socket);

    if(answer){
        isSender = true;
        file_sender.SendFileData(); 
        file_sender.Clean();
    }
    else{
        emit message("The user declined the request to send the file");
    }

    //WARNING: ПРЕВЫШЕНИЕ ОЖИДАНИЯ ОТВЕТА В recive_unique ВРЕМЕНИ TIMEOUT ПРИВЕДЕТ К НЕПРАВИЛЬНОЙ
    //РАБОТЕ ПРОГРАММЫ - ИСПРАВИТЬ ПОЗЖЕ

    file_sender.Clean();
    isSender = false;

}

User* User::getUser(qintptr sockDS)
{
    auto user_it = std::find_if(connected_users.begin(),connected_users.end(),[sockDS](const User* p_user){
        return p_user->sockets.info_socket->socketDescriptor() == sockDS;
    });
    return user_it != connected_users.end() ? *user_it : nullptr;
}

inline size_t User::getId(quint16 port,QHostAddress IP)
{
    std::string str_id = IP.toString().toStdString() + QString::number(port).toStdString();
    str_id.erase(std::remove(str_id.begin(),str_id.end(),'.'),str_id.end());
    return std::hash<std::string>()(str_id);
}

User *User::getUserFromId(size_t id)
{
    for(auto* user: connected_users)
        if(user->info.Id == id)
            return user;

    return nullptr;
}


void User::newConnectionSlot()
{
    static bool users_info_socket_connected = false;
    static User* user = nullptr;

    if(!users_info_socket_connected){
        QTcpSocket* new_user_socket = nextPendingConnection();
        user = new  User(new_user_socket);
        connect(user->sockets.info_socket,&QTcpSocket::readyRead,this,&User::ReadyReadSlot);



        user->info.Id = recive_unique<size_t>(new_user_socket);
        qDebug() <<  user->info.Id << "\trecived";
        users_info_socket_connected = true;

        User* _user = user;
        connect(user->sockets.info_socket,&QTcpSocket::disconnected,user->sockets.info_socket,[this,_user](){
            emit UserDisconnected(QString::number(_user->info.Id));
        });

    }

    else{
        user->sockets.data_socket = nextPendingConnection();
        connect(user->sockets.data_socket,&QTcpSocket::readyRead,this,&User::FileDataArrived);

        qDebug() << "connected " << user->info.IP << "\t" << user->info.Id;
        emit UserConnected(user->info.IP.toString(),QString::number(user->info.Id));
        connected_users.push_back(user);
        user = nullptr;
        users_info_socket_connected = false;
    }


}

void User::ReadyReadSlot()
{
    QTcpSocket* sender_sock =  qobject_cast<QTcpSocket*>(sender());
    User* user = getUser(sender_sock->socketDescriptor());

    MessageType msg_type = ReciveMessage(sender_sock);
    qDebug() <<"Message type: " <<  msg_type;

    if(!user)
        return;

    else if(msg_type == MessageType::CONNECTION_MESSAGE)
           return;

    else if(msg_type == MessageType::ANSWER_BOOL_MESSAGE)
        return;

    else if(msg_type == MessageType::FILE_INFO_MESSAGE)
       ProcessFileInfoMessage(user);

    else if(msg_type == MessageType::STOP_TRANSFERING_MESSAGE){
        sender_sock->readAll();
        file_sender.stoped();
    }

}

void User::FileDataArrived()
{
    if(FileArrivedSlotLock)
        return;

    if(!file_sender.getRecivingGoingStatus()){
        qobject_cast<QTcpSocket*>(sender())->readAll();
        return;
    }

    if(file_sender.GetSizeOfReciveData() < file_sender.getReciveFileInfo().size)
        file_sender.ReciveAvailableFileData(file_sender.getReciveFileInfo().size);

    if(file_sender.GetSizeOfReciveData() == file_sender.getReciveFileInfo().size){
        file_sender.Clean();
        qDebug() << "end reciving";
    }
}

void User::ConnectSlot(QString port, QString IP)
{
    ConnectTo(static_cast<quint16>(port.toUInt()),QHostAddress(IP));
}

void User::NewFileSendSlot(QString path,QString _id)
{
    qDebug() << "send" <<  path << "\n";
    size_t id = _id.toULongLong();
    selected_user = getUserFromId(id);
    SendFileToUser(selected_user,path);
}

void User::UserDisconnectedSlot(QString id)
{
    size_t _id = id.toULongLong();
    User* disconnected_user = getUserFromId(_id);
    disconnected_user->sockets.data_socket->deleteLater();
    disconnected_user->sockets.info_socket->deleteLater();


    auto user_it = std::find(connected_users.begin(),connected_users.end(),disconnected_user);

    if(selected_user == disconnected_user && (file_sender.getRecivingGoingStatus() ||
            file_sender.getSendingGoingStatus())){
        file_sender.stoped();
    }

    delete *user_it;
    connected_users.erase(user_it);
    qDebug() << "user" << _id << "disconnected";
}

void User::stoped()
{
    SendMessageToUser_T(selected_user,MessageType::STOP_TRANSFERING_MESSAGE,"");
    file_sender.stoped();
}

void User::SendMessageToUser(User *user,  MessageType msg_type, QByteArray msg_data)
{
    int type = static_cast<int>(msg_type);
    QByteArray message = QByteArray(reinterpret_cast<char*>(&type),sizeof(int)) + msg_data;
    user->sockets.info_socket->write(message);
}

User::MessageType User::ReciveMessage(QTcpSocket *socket)
{
   int msgkey = *reinterpret_cast<int*>(socket->read(sizeof(int)).data());
   return static_cast<MessageType>(msgkey);
}


void User::ProcessFileInfoMessage(User *user)
{
    FileSender::FileInfo recived_file_info = file_sender.ReciveFileInfo(user->sockets.info_socket);

    QString senderIP = QHostAddress(user->sockets.info_socket->localAddress().toIPv4Address()).toString();
    //*****************code for agreement or disagreement************

    QString* save_dir = new QString();
    emit ask_to_recive_file(save_dir,QString(recived_file_info.name),recived_file_info.size,senderIP);
    bool answer = (*save_dir != "");
    SendMessageToUser_T(user,MessageType::ANSWER_BOOL_MESSAGE,answer);

    if(!answer){
        delete save_dir;
        return;
    }
    else {
        QString path = *save_dir +  '/' + QString(recived_file_info.name);
        if(!file_sender.Open(path,QFile::WriteOnly,user->sockets.data_socket)){
           Error("Cant open file: " + path);
           //ДОБАВИТЬ СЮДА ОТКАЗ ОТ ОТРПАВКИ
        }
        file_sender.SetReciveFileInfo(recived_file_info);
        selected_user = user;
        qDebug() << "start reciving....";
        file_sender.StartReciving();
        delete save_dir;
    }
     //*****************code for agreement or disagreement************
}

