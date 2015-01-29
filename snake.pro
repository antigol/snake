#-------------------------------------------------
#
# Project created by QtCreator 2012-09-24T19:32:10
#
#-------------------------------------------------

QT       += core gui

TARGET = snake
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    snakescene.cpp \
    newgamedialog.cpp

HEADERS  += mainwindow.h \
    snakescene.h \
    newgamedialog.h

FORMS += \
    newgamedialog.ui

RESOURCES += \
    res.qrc

unix {
    menu.files    = snake.desktop
    menu.path     = /usr/share/applications

    share.files   = snake-icon.png
    share.path    = /share/snake

    target.path   = /usr/bin

    INSTALLS += target menu share
}
