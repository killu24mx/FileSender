#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inputwin.h"
#include "loaddataprogressdialog.h"
#include "newdatamessage.h"
#include "userwidgetscrolllist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void design();
    friend class SignalCreator;

signals:
    void SendButton_pressed_signal(QString file_path,QString id);

public slots:

    void addUserToScrollList(QString IP,QString id);
    void SelectedUserChanged(size_t new_selected_user_id){
        selected_user_id = new_selected_user_id;
    }

private slots:
    void on_BrowserButton_pressed();

    void on_ConnectButton_pressed();

    void on_SendButton_clicked();
public slots:

private:
    Ui::MainWindow *ui;
    InputWin* input_win;
    LoadDataProgressDialog* progress_dialog;
    UserWidgetScrollList* sl;
    size_t selected_user_id;
};


#endif // MAINWINDOW_H
