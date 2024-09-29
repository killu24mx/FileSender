#ifndef USERWIDGETSCROLLLIST_H
#define USERWIDGETSCROLLLIST_H

#include <QWidget>

#include "userwidget.h"
class UserWidgetScrollList : public QWidget
{
    Q_OBJECT
public:
    explicit UserWidgetScrollList(QWidget *parent = nullptr);
    void addUser(QString ip,size_t id);
    ~UserWidgetScrollList();

    UserWidget* GetUserWidget(size_t id) {
    auto it =  std::find_if(user_widgets.begin(),user_widgets.end(),[id]( UserWidget* user_w){
        return user_w->id == id;
    });
    return *it;
    }

    void EraseUserWidget(UserWidget* user_w);

public slots:
    void PressedOnWidget(size_t id);
    void UserDisconnected(QString id);

signals:
    void SelectedUserChanged(size_t id);
private:
    std::vector<UserWidget*> user_widgets;
    std::vector<QFrame*> list_of_lines;

    size_t selected_user_id = 0;

    size_t getLineIndex(UserWidget* bt);
    void MoveUp(UserWidget* userbt);

    // QWidget interface

};

#endif // USERWIDGETSCROLLLIST_H
