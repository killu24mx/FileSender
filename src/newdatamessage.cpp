#include "include/newdatamessage.h"
#include "ui_newdatamessage.h"

NewDataMessage::NewDataMessage(QString& file_path,QString file_name,qint64 size,QString userIP,QWidget *parent):
    QDialog(parent),
    ui(new Ui::NewDataMessage),
    file_path(file_path)
{
    this->setModal(true);
    ui->setupUi(this);
    ui->InfoText->setText("User "+ userIP + " try to send you a file " + ConvertDataSizeToString(size) +
                          ":\n"+file_name +
                          ".\nRecive it?");

    connect(ui->SaveButton,&QPushButton::clicked,this,&NewDataMessage::accept);
    connect(ui->CancelButton,&QPushButton::clicked,this,&NewDataMessage::reject);
    setFixedSize(650,215);
}

NewDataMessage::~NewDataMessage()
{
    delete ui;
}

void NewDataMessage::on_BrowserButton_clicked()
{
    file_path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->PathTextEdit->setText(file_path);
}


QString ConvertDataSizeToString(qint64 size){
    const qint64 KB = 1024;
    const qint64 MB = 1024 * KB;
    const qint64 GB = 1024 * MB;

    if(size < KB){
        return QString::number(size) + " B";
    }
    else if(size < MB){
        return QString::number(((qreal)size / (qreal)KB),'g',3) + "KB";
    }
    else{
        return QString::number(((qreal)size / (qreal)GB),'g',3) + "GB";
    }
}
