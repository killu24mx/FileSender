QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/filesender.cpp \
    src/loaddataprogressdialog.cpp \
    src/main.cpp \
    src/inputwin.cpp \
    src/mainwindow.cpp \
    src/messagedialog.cpp \
    src/newdatamessage.cpp \
    src/signalcreator.cpp \
    src/threadcontroller.cpp \
    src/user.cpp \
    src/userwidget.cpp \
    src/userwidgetscrolllist.cpp

INCLUDEPATH += "."

HEADERS += \
    include/filesender.h \
    include/inputwin.h \
    include/loaddataprogressdialog.h \
    include/mainwindow.h \
    include/messagedialog.h \
    include/newdatamessage.h \
    include/signalcreator.h \
    include/threadcontroller.h \
    include/user.h \
    include/userwidget.h \
    include/userwidgetscrolllist.h

FORMS += \
    forms/inputwin.ui \
    forms/loaddataprogressdialog.ui \
    forms/mainwindow.ui \
    forms/messagedialog.ui \
    forms/newdatamessage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
