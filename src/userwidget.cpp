#include "include/userwidget.h"

UserWidget::UserWidget(QWidget *parent) : QWidget(parent)
{
    isSelected = false;
    ip_label = new QLabel(this);

    //"fantasy"
    //"cursive"
    QString font_name = "fantasy";
    int font_size = 10;

    QFont font(font_name,font_size,QFont::QFont::DemiBold,false);

    ip_label->setFont(font);
    ip_label->setStyleSheet("Color:rgb(255, 255, 255)");

    id_label =  new QLabel(this);

    id_label->setFont(font);
    id_label->setStyleSheet("Color:rgb(255, 255, 255)");
}



void UserWidget::setUserData(QString ip, size_t id)
{
    ip_label->setText("IP: " + ip);
    id_label->setText("ID: " + QString::number(id));
    this->id = id;
}


void UserWidget::setGeometry(int x, int y, int w, int h)
{
    QWidget::setGeometry(x,y,w,h);
    ip_label->setGeometry(w * 0.04,h*0.15,w/2,h/3);
    id_label->setGeometry(w*0.04,h*0.60,w/2,h/3);

    show();
}

void UserWidget::mousePressEvent(QMouseEvent *event)
{
    emit PressedOn(this->id);
}


void UserWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);


    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);


    if(isSelected){
        QPen pen(QColor(255, 136, 0),Qt::SolidLine);
        painter.setPen(pen);
        painter.setBrush(QColor(255, 136, 0));
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter.drawEllipse(4,4,geometry().height()/3,geometry().height()/3);
    }

}
