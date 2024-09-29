#include "include/userwidgetscrolllist.h"

UserWidgetScrollList::UserWidgetScrollList(QWidget *parent) : QWidget(parent)
{

}

void UserWidgetScrollList::addUser(QString ip, size_t id)
{
    UserWidget* user_w = new UserWidget(this);


    user_w->setGeometry(0,0,width(),width()/15);
    user_w->setStyleSheet("background: rgb(79, 79, 79)");

    user_w->setUserData(ip,id);
    if(user_widgets.size()){
         auto last_iser_butt = user_widgets[user_widgets.size() - 1];
         user_w->move(last_iser_butt->x(),last_iser_butt->y() + last_iser_butt->height() + 1);
         if(user_w->y() + 2* width()/7 >= height())
             resize(width(),2*height());

    }
    else{
        user_w->move(0,0);
    }

    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFixedHeight(1);
    line->setGeometry(0,user_w->y() + user_w->height(),user_w->width(),1);

    list_of_lines.push_back(line);
    user_widgets.push_back(user_w);
    connect(user_w,&UserWidget::PressedOn,this,&UserWidgetScrollList::PressedOnWidget);
    line->show();
}

UserWidgetScrollList::~UserWidgetScrollList()
{
    while(user_widgets.size()){
        delete user_widgets[0];
        user_widgets.erase(user_widgets.begin());
    }

    while(list_of_lines.size()){
        delete list_of_lines[0];
        list_of_lines.erase(list_of_lines.begin());
    }
}

void UserWidgetScrollList::EraseUserWidget(UserWidget *user_w)
{
    size_t line_index = getLineIndex(user_w);
    list_of_lines[line_index]->hide();
    delete list_of_lines[line_index];
    list_of_lines.erase(list_of_lines.begin() + line_index);

    user_widgets.erase(std::find(user_widgets.begin(),user_widgets.end(),user_w));
    user_w->hide();
    delete user_w;

    for(size_t i = line_index; i < user_widgets.size(); i++){
        MoveUp(user_widgets[i]);
    }
}

void UserWidgetScrollList::PressedOnWidget(size_t id)
{
    UserWidget* user_w = GetUserWidget(id);
    if(!user_w->isSelected){
        user_w->isSelected = true;
        user_w->update();
        selected_user_id = id;
        emit SelectedUserChanged(selected_user_id);

        for(auto* w: user_widgets){
            if(w->id != id && w->isSelected){
                w->isSelected = false;
                w->update();
            }
        }
    }
    else{
        user_w->isSelected = false;
        user_w->update();
        selected_user_id = 0;
        emit SelectedUserChanged(selected_user_id);
    }
}

void UserWidgetScrollList::UserDisconnected(QString id)
{
    size_t _id = id.toULongLong();
    UserWidget* user_w = GetUserWidget(_id);
    EraseUserWidget(user_w);

}

inline size_t UserWidgetScrollList::getLineIndex(UserWidget *user_w)
{
    auto it = std::find(user_widgets.begin(),user_widgets.end(),user_w);
    size_t line_index = it - user_widgets.begin();
    return line_index;
}

void UserWidgetScrollList::MoveUp(UserWidget *user_w)
{
    int offset = user_w->height();

    user_w->move(user_w->x(),user_w->y() - offset);
    auto line =  list_of_lines[getLineIndex(user_w)];
    line->move(line->x(),line->y() - offset);
}

