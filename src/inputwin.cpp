#include "include/inputwin.h"
#include "ui_inputwin.h"

InputWin::InputWin(QWidget *parent): QMainWindow(parent), ui(new Ui::InputWin)
{
    ui->setupUi(this);
    setFixedSize(355,360);
}


InputWin::~InputWin()
{
    delete ui;
}

void InputWin::ConnectionResSlot(QString status,bool res)
{
      ui->ConnectRes->setStyleSheet(res ? "color : green" : "color : red");
      ui->ConnectRes->setText(status);
}


void InputWin::on_ConnectButton_clicked()
{
    QString IP = ui->IPLineEdit->text();
    QString port = ui->PortLineEdit->text();
    emit Connect_Button_Clicked(port,IP);
}

