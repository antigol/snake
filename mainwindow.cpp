#include "mainwindow.h"
#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Snake");
    setWindowIcon(QIcon(":/snake-icon.png"));

    _view = new QGraphicsView(this);
    _view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setCentralWidget(_view);

    _snake = new SnakeScene(this);
    _view->setScene(_snake);
    _view->setTransform(QTransform::fromScale(0.9, 0.9));

    QToolBar *toolbar = addToolBar("Snake");
    toolbar->setIconSize(QSize(32, 32));

    QAction *action = toolbar->addAction(QIcon(":/snake-icon.png"), "New", this, SLOT(newgame()));
    action->setShortcut(QKeySequence::New);

    action = toolbar->addAction("Full screen", this, SLOT(fullscreen()));
    action->setShortcut(QKeySequence("F11"));

    action = toolbar->addAction("Pause", _snake, SLOT(pause()));
    action->setShortcut(QKeySequence("P"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    _snake->setSceneRect(_view->rect());
}

void MainWindow::newgame()
{
    NewGameDialog d;
    if (d.exec()) {
        _snake->newGame(d.ui->widthSpinBox->value(), d.ui->heigthSpinBox->value(), d.ui->speedSpinBox->value());
    }
}

void MainWindow::fullscreen()
{
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}
