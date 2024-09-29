#include "include/signalcreator.h"


bool SignalCreator::CreateSignalLinks(MainWindow *main_win,User* user){
    bool res = true;
    res = res && connect(main_win->input_win,&InputWin::Connect_Button_Clicked,user,&User::ConnectSlot);

    res = res && connect(user,&User::ConnectionRes,main_win->input_win,&InputWin::ConnectionResSlot);

    res = res && connect(main_win,&MainWindow::SendButton_pressed_signal,user,&User::NewFileSendSlot);

    res = res && connect(&user->file_sender,&FileSender::StartFileSending,
                         main_win->progress_dialog,[main_win](){
         main_win->progress_dialog->run("Sending");
    });

    res = res && connect(&user->file_sender,&FileSender::FinishFileSending,
            main_win->progress_dialog,[main_win](){
        main_win->progress_dialog->finish("Sending");
   });

    res = res && connect(&user->file_sender,&FileSender::StartFileReciving,
            main_win->progress_dialog,[main_win](){
        main_win->progress_dialog->run("Reciving");
   });

    res = res && connect(&user->file_sender,&FileSender::FinishFileReciving,
                    main_win->progress_dialog,[main_win](){
        main_win->progress_dialog->finish("Reciving");
   });

    res = res && connect(&user->file_sender,&FileSender::ProgressValueChanged,main_win->progress_dialog
            ,&LoadDataProgressDialog::progressValueChangedSlot);

    res = res && connect(user,&User::stop,&user->file_sender,&FileSender::stoped);

    res = res && connect(main_win->progress_dialog,&LoadDataProgressDialog::stop,user,&User::stoped);

    res = res && connect(user,&User::UserConnected,main_win,&MainWindow::addUserToScrollList);

    res = res && connect(user,&User::UserDisconnected,main_win->sl,&UserWidgetScrollList::UserDisconnected);
    return res;
}
