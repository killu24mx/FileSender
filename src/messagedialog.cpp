#include "include/messagedialog.h"
#include "ui_messagedialog.h"

MessageDialog::MessageDialog(QString msg,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageDialog)
{
    ui->setupUi(this);
    ui->MsgLabel->setText(msg);
    connect(ui->okButton,&QPushButton::clicked,this,&MessageDialog::accept);
    setFixedSize(436,171);
}

MessageDialog::~MessageDialog()
{
    delete ui;
}
