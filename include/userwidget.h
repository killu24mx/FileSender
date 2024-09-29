#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QStyleOption>
class UserWidget : public QWidget
{
    Q_OBJECT
public:
    friend class UserWidgetScrollList;

    explicit UserWidget(QWidget *parent = nullptr);
    void setUserData(QString ip,size_t id);
    int getUserId()const {return id;}


    void setGeometry(int x,int y,int w,int h);

signals:
    void PressedOn(size_t id);
private:
    bool isSelected;
    QLabel* ip_label;
    QLabel* id_label;
    size_t id;


    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // USERWIDGET_H
