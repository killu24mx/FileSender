#ifndef INPUTWIN_H
#define INPUTWIN_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class InputWin; }
QT_END_NAMESPACE

class InputWin : public QMainWindow
{
    Q_OBJECT

public:
    InputWin(QWidget *parent = nullptr);
    ~InputWin();

public slots:
     void ConnectionResSlot(QString status,bool res);
private slots:
    void on_ConnectButton_clicked();

signals:
    void Connect_Button_Clicked(QString,QString);
    void UserConnected(QString IP,QString port);


private:
    Ui::InputWin *ui;

};
#endif // INPUTWIN_H
