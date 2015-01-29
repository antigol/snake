#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QGraphicsView>
#include "snakescene.h"
#include "newgamedialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void resizeEvent(QResizeEvent *);

private slots:
    void newgame();
    void fullscreen();

private:
    QGraphicsView *_view;
    SnakeScene *_snake;
};

#endif // MAINWINDOW_H
