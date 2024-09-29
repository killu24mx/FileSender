#include "include/loaddataprogressdialog.h"
#include "ui_loaddataprogressdialog.h"

LoadDataProgressDialog::LoadDataProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadDataProgressDialog)
{
    ui->setupUi(this);
    setModal(true);
    setProgressValue(0);

    connect(this,&LoadDataProgressDialog::rejected,this,[this](){
        if(!ProgressDialogSendRejectedSingal)
            emit this->stop();
    });
    setFixedSize(552,206);

}

LoadDataProgressDialog::~LoadDataProgressDialog()
{
    delete ui;
}

void LoadDataProgressDialog::setProgressValue(qreal percents)
{
    ui->ProgressLabel->setText("Progress: " + QString::number(percents) + "%");
    ui->progressBar->setValue(percents);
}

void LoadDataProgressDialog::progressValueChangedSlot(qreal new_progress)
{
    setProgressValue(new_progress);
}

void LoadDataProgressDialog::run(QString loadname)
{
    ui->LoadName->setText(loadname);
    show();
}

void LoadDataProgressDialog::finish(QString loadname)
{
    setProgressValue(0);
    ProgressDialogSendRejectedSingal = true;
    reject();
    ProgressDialogSendRejectedSingal = false;

}



void LoadDataProgressDialog::on_CancelButton_clicked()
{
    setProgressValue(0);
    reject();

}

