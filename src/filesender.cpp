#include "include/filesender.h"

FileSender::FileSender(): socket(nullptr),file(nullptr),send_data(0)
  ,recive_data(0),file_size(0){


}

FileSender::~FileSender()
{
    Clean();
}


bool FileSender::Open(QString path, QIODevice::OpenMode open_mode,QTcpSocket* socket)
{
    Clean();
    file = new QFile(path);

    if(file->open(open_mode)){
        info = getFileInfo();
        this->socket = socket;
        file_size = file->size();
        return true;
    }
    Clean();
    return false;
}

void FileSender::SendFileInfo()
{
   QByteArray info_buffer(reinterpret_cast<char*>(&info),sizeof(info));
   this->socket->write(info_buffer);
}

FileSender::FileInfo FileSender::ReciveFileInfo(QTcpSocket *_socket){
    QByteArray info_buffer = _socket->read(sizeof(FileInfo));
    FileInfo file_info = *reinterpret_cast<FileInfo*>(info_buffer.data());
    return file_info;
}


FileSender::FileInfo FileSender::getFileInfo()
{
    QFileInfo q_info(*this->file);
    return ConvertFromQFileInfo(q_info);
}


void FileSender::SendPartOfFile()
{
    qint64 avaliable_size = file->bytesAvailable();
    qint64 size_for_send = avaliable_size >= SIZE_OF_PART ? SIZE_OF_PART : avaliable_size;
    send_data += socket->write(file->read(size_for_send));

}

FileSender::FileInfo FileSender::ConvertFromQFileInfo(QFileInfo qinfo){
    FileInfo info;
    std::string file_name = qinfo.fileName().toStdString();
    strcpy(info.name,file_name.c_str());
    info.size = qinfo.size();
    return info;
}

void FileSender::SendFileData()
{
    emit StartFileSending();
    sending_is_going = true;
    qDebug() << "sending_is_going: " <<  sending_is_going;
    while(!file->atEnd()){
         SendPartOfFile();
         emit ProgressValueChanged((qreal)send_data/(qreal)file_size * 100);
         qDebug() << "send progress: " << (qreal)send_data/(qreal)file_size * 100;
         QCoreApplication::processEvents();
         if(!sending_is_going)
             return;
    }

    if(sending_is_going){ emit FinishFileSending();}
    sending_is_going = false;
}

void FileSender::ReciveAvailableFileData(qint64 general_file_size)
{
    while(socket->bytesAvailable() >= SIZE_OF_PART)
        recive_data += file->write(socket->read(SIZE_OF_PART));

    recive_data += file->write(socket->readAll());
    ProgressValueChanged((qreal)recive_data/(qreal)general_file_size * 100);

    if(recive_data == general_file_size){
        emit FinishFileReciving();
        reciving_is_going = false;
    }

    qDebug() << "recive progress: " << (qreal)recive_data/(qreal)general_file_size * 100;
    qDebug() << "general recive_data\t" << recive_data;

}


void FileSender::Clean(bool delete_file)
{
    if(file){
        file->close();
        if(delete_file) file->remove();
        delete file;
        file = nullptr;
    }

    send_data = 0;
    recive_data = 0;
    socket = nullptr;
    if(socket)socket->readAll();
    file_size = 0;
    recive_file_info.size = 0;

}
