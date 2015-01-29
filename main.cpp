#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(300, 300);
    w.show();
    
    qsrand(a.applicationPid());

    return a.exec();
}
