#include <QDebug>
#include <QFileDialog>

#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow),
    progress_dialog(new LoadDataProgressDialog(this))
{
    ui->setupUi(this);
    setFixedSize(991,419);
    input_win = new InputWin(this);
    sl = new UserWidgetScrollList();
    ui->scrollArea->setWidget(sl);

    sl->setGeometry(ui->scrollArea->x(),
                    ui->scrollArea->y(),
                    ui->scrollArea->width(),
                    ui->scrollArea->height()*0.7);
    sl->show();

    selected_user_id = 0;

    connect(sl,&UserWidgetScrollList::SelectedUserChanged,this,&MainWindow::SelectedUserChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(input_win) delete input_win;
    delete progress_dialog;
}

void MainWindow::addUserToScrollList(QString IP, QString id)
{
    sl->addUser(IP,id.toULongLong());
}




void MainWindow::on_BrowserButton_pressed()
{
     QString path = QFileDialog::getOpenFileName(this, "Open File", "/home/", ";;All Files (*.*)");
     ui->PathTextEdit->setText(path);
}


void MainWindow::on_ConnectButton_pressed()
{
   input_win->show();
}


void MainWindow::on_SendButton_clicked()
{
    if(selected_user_id && ui->PathTextEdit->toPlainText() != ""){
        emit MainWindow::SendButton_pressed_signal(ui->PathTextEdit->toPlainText(),QString::number(selected_user_id));
    }

}

