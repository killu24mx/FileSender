#ifndef FILESENDER_H
#define FILESENDER_H

#include <QFile>
#include <QFileInfo>
#include <QTcpSocket>
#include <QThread>
#include <QCoreApplication>
#include "loaddataprogressdialog.h"

constexpr int TIME_OUT = 300000; //waiting new data on socket time;

class FileSender: public QObject
{
Q_OBJECT

public:
    FileSender();

    ~FileSender();

    bool Open(QString path,QFile::OpenMode open_mode,QTcpSocket* socket);
    void Clean(bool delete_file = false);

    //*******************FILEINFO***************************
    struct FileInfo{
        qint64 size;
        char name[200];
    };

    void SendFileInfo();
    FileInfo getFileInfo();
    void SetReciveFileInfo(FileInfo info){ this->recive_file_info = info;}
    FileInfo getReciveFileInfo(){return this->recive_file_info;}
    //*******************FILEINFO***************************

    void SendFileData();
    void ReciveAvailableFileData(qint64 general_file_size);
    static FileInfo ReciveFileInfo(QTcpSocket* _socket);

    qint64 GetSizeOfSendData(){return send_data;}
    qint64 GetSizeOfReciveData(){return recive_data;}
    qint64 GetFileSize(){return file_size;}
    bool   getSendingGoingStatus(){return sending_is_going;}
    bool   getRecivingGoingStatus(){return reciving_is_going;}


    void StartReciving(){
        emit StartFileReciving();
        reciving_is_going = true;
    }


public slots:
    void stoped(){
        bool isReciver = !sending_is_going &&  reciving_is_going;
        if(sending_is_going)  emit FinishFileSending();
        if(reciving_is_going) emit FinishFileReciving();
        sending_is_going = false;
        reciving_is_going = false;
        qDebug() << "call stoped";
        Clean(isReciver);
    }

signals:
    void ProgressValueChanged(qreal new_progress);
    void StartFileReciving();
    void FinishFileReciving();
    void StartFileSending();
    void FinishFileSending();


private:
    QFile* file;
    QTcpSocket* socket;
    qint64 send_data;
    qint64 recive_data;
    qint64 file_size;
    FileInfo recive_file_info;

    bool sending_is_going = false;
    bool reciving_is_going = false;

    const qint64 SIZE_OF_PART = 60 * 1024; //60kb
    FileInfo info;

    void SendPartOfFile();

    static FileInfo ConvertFromQFileInfo(QFileInfo qinfo);
};

template <typename T>
T recive_unique(QTcpSocket* socket){
    socket->waitForReadyRead(TIME_OUT);
    QByteArray buff = socket->read(sizeof(T));
    return *reinterpret_cast<T*>(buff.data());
}

template <typename T>
void send_unique(T value,QTcpSocket* socket){
    socket->write(reinterpret_cast<char*>(&value),sizeof(T));
}


#endif // FILESENDER_H
