#ifndef LOADDATAPROGRESSDIALOG_H
#define LOADDATAPROGRESSDIALOG_H

#include <QDialog>
#include <QDebug>
namespace Ui {
class LoadDataProgressDialog;
}

class LoadDataProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadDataProgressDialog(QWidget *parent = nullptr);
    ~LoadDataProgressDialog();

    void setProgressValue(qreal percents);

signals:
    void stop();
public slots:

    void progressValueChangedSlot(qreal new_progress);
    void run(QString);
    void finish(QString);


private slots:

    void on_CancelButton_clicked();

private:
    Ui::LoadDataProgressDialog *ui;

    bool ProgressDialogSendRejectedSingal = false;
};

#endif // LOADDATAPROGRESSDIALOG_H
