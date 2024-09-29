#ifndef NEWDATAMESSAGE_H
#define NEWDATAMESSAGE_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class NewDataMessage;
}

class NewDataMessage : public QDialog
{
    Q_OBJECT

public:
    explicit NewDataMessage(QString& file_path,QString file_name,
                            qint64 size,QString userIP,QWidget *parent = nullptr);
    ~NewDataMessage();

private slots:
    void on_BrowserButton_clicked();

private:
    Ui::NewDataMessage *ui;
    QString& file_path;  
};

QString ConvertDataSizeToString(qint64 size);


#endif // NEWDATAMESSAGE_H
