#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class MessageDialog;
}

class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDialog(QString msg,QWidget *parent = nullptr);
    ~MessageDialog();

private:
    Ui::MessageDialog *ui;
};

#endif // MESSAGEDIALOG_H
