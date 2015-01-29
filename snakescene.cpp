#include "snakescene.h"
#include <QKeyEvent>
#include <QDebug>
#include <QGraphicsTextItem>
#include <QFontMetrics>
#include <QRadialGradient>

SnakeScene::SnakeScene(QObject *parent) :
  QGraphicsScene(parent)
{
  _timerId = 0;
  _interval = 0;
}

void SnakeScene::newGame(int w, int h, int timer)
{
  _width = w;
  _heigth = h;

  _grid.clear();
  for (int y = 0; y < _heigth; ++y) {
    for (int x = 0; x < _width; ++x) {
      _grid[QPoint(x, y)] = Empty;
    }
  }
  /*
      #######
      #     # w = 5
      # x x # x1 = 1
      # x x # x2 = 4
      # x x #
      #     # x1 = (w-2)/3
      ####### x2 = 2*(w-2)/3 + 1
     */
  int y = (_width - 3) / 2;
  int x1 = (_width - 2) / 3;
  int x2 = 2 * (_width - 2) / 3 + 1;

  _grid[QPoint(x1, y)] = Snake1;
  _grid[QPoint(x1, y+1)] = Snake1;
  _grid[QPoint(x1, y+2)] = Snake1;
  _snake1.clear();
  _snake1 << QPoint(x1,y+2) << QPoint(x1,y+1) << QPoint(x1,y);
  _newDirection1 = _lstDirection1 = Up;
  _score1 = 0;
  _gameOver1 = false;

  _grid[QPoint(x2, y)] = Snake2;
  _grid[QPoint(x2, y+1)] = Snake2;
  _grid[QPoint(x2, y+2)] = Snake2;
  _snake2.clear();
  _snake2 << QPoint(x2,y+2) << QPoint(x2,y+1) << QPoint(x2,y);
  _newDirection2 = _lstDirection2 = Up;
  _score2 = 0;
  _gameOver2 = false;

  addRandomFood();

  if (_timerId)
    killTimer(_timerId);
  _timerId = startTimer(timer);
  _interval = timer;
}

void SnakeScene::pause()
{
  if (_timerId) {
    killTimer(_timerId);
    _timerId = 0;
  } else {
    _timerId = startTimer(_interval);
  }
}

void SnakeScene::keyPressEvent(QKeyEvent *event)
{
  switch (event->key()) {
  case Qt::Key_W:
    _direction1 << Up;
    break;
  case Qt::Key_S:
    _direction1 << Dn;
    break;
  case Qt::Key_A:
    _direction1 << Lt;
    break;
  case Qt::Key_D:
    _direction1 << Rt;
    break;
  case Qt::Key_Up:
    _direction2 << Up;
    break;
  case Qt::Key_Down:
    _direction2 << Dn;
    break;
  case Qt::Key_Left:
    _direction2 << Lt;
    break;
  case Qt::Key_Right:
    _direction2 << Rt;
    break;
  }
}

void SnakeScene::timerEvent(QTimerEvent *)
{
  if (_snake1.isEmpty() || _snake2.isEmpty())
    return;

  _newDirection1 = _lstDirection1;
  while (!_direction1.isEmpty()) {
    Direction d = _direction1.takeFirst();
    if ((_lstDirection1 == Up && d != Dn)
        || (_lstDirection1 == Dn && d != Up)
        || (_lstDirection1 == Lt && d != Rt)
        || (_lstDirection1 == Rt && d != Lt))
    {
      _newDirection1 = d;
      break;
    }
  }
  _newDirection2 = _lstDirection2;
  while (!_direction2.isEmpty()) {
    Direction d = _direction2.takeFirst();
    if ((_lstDirection2 == Up && d != Dn)
        || (_lstDirection2 == Dn && d != Up)
        || (_lstDirection2 == Lt && d != Rt)
        || (_lstDirection2 == Rt && d != Lt))
    {
      _newDirection2 = d;
      break;
    }
  }

  QPoint head1 = _snake1.last();
  switch (_newDirection1) {
  case Up:
    head1.ry()--;
    if (head1.y() == -1)
      head1.ry() = _heigth - 1;
    break;
  case Dn:
    head1.ry()++;
    if (head1.y() == _heigth)
      head1.ry() = 0;
    break;
  case Lt:
    head1.rx()--;
    if (head1.x() == -1)
      head1.rx() = _width - 1;
    break;
  case Rt:
    head1.rx()++;
    if (head1.x() == _width)
      head1.rx() = 0;
    break;
  }
  QPoint head2 = _snake2.last();
  switch (_newDirection2) {
  case Up:
    head2.ry()--;
    if (head2.y() == -1)
      head2.ry() = _heigth - 1;
    break;
  case Dn:
    head2.ry()++;
    if (head2.y() == _heigth)
      head2.ry() = 0;
    break;
  case Lt:
    head2.rx()--;
    if (head2.x() == -1)
      head2.rx() = _width - 1;
    break;
  case Rt:
    head2.rx()++;
    if (head2.x() == _width)
      head2.rx() = 0;
    break;
  }

  // food ahead
  bool food1 = _grid[head1] == Food && !_gameOver1;
  bool food2 = _grid[head2] == Food && !_gameOver2;

  _score1 += food1?1:0;
  _score2 += food2?1:0;

  // erase tail
  if (!food1 && !_gameOver1) {
    QPoint tail = _snake1.takeFirst();
    _grid[tail] = Empty;
  }
  if (!food2 && !_gameOver2) {
    QPoint tail = _snake2.takeFirst();
    _grid[tail] = Empty;
  }

  // Snyc1

  // collision
  bool collision1 = _grid[head1] == Snake1 || _grid[head1] == Snake2;
  bool collision2 = _grid[head2] == Snake1 || _grid[head2] == Snake2;

  // loose head on collision
  if (collision1 && !_gameOver1)
    _snake1.removeLast();
  if (collision2 && !_gameOver2)
    _snake2.removeLast();

  _gameOver1 |= collision1;
  _gameOver2 |= collision2;

  // add head
  if (!collision1 && !_gameOver1) {
    _grid[head1] = Snake1;
    _snake1 << head1;
  }
  if (!collision2 && !_gameOver2) {
    _grid[head2] = Snake2;
    _snake2 << head2;
  }

  // same head
  if (head1 == head2) {
    _gameOver1 = _gameOver2 = true;
  }

  _lstDirection1 = _newDirection1;
  _lstDirection2 = _newDirection2;

  if (food1 || food2)
    addRandomFood();

  if ((_gameOver1 && _score2 > _score1) || (_gameOver2 && _score1 > _score2))
    _gameOver1 = _gameOver2 = true;



  //////////////////////////////
  // draw screen
  //////////////////////////////
  clear();


  qreal d = qMin((width() - 40) / qreal(_width), height() / qreal(_heigth));
  qreal ox = (width() - d * _width) / 2.0;
  qreal oy = (height() - d * _heigth) / 2.0;

  addRect(ox, oy, _width * d, _heigth * d);

  QFont font;
  if (_gameOver1 && _gameOver2) {
    font.setPixelSize(50);
    QString str;
    if (_score1 > _score2) {
      str = "blue WIN !!";
    } else if (_score2 > _score1) {
      str = "red WIN !!";
    } else {
      str = "no winer";
    }
    QGraphicsTextItem *txt = addText(str, font);
    txt->setPos(width() / 2.0 - QFontMetrics(font).width(txt->toPlainText()) / 2.0, height() / 2.0);
    txt->setZValue(1);
  }
  font.setPixelSize(30);

  QGraphicsTextItem *txt = addText(QString("%1").arg(_score1), font);
  int tw = QFontMetrics(font).width(txt->toPlainText());
  txt->setDefaultTextColor(Qt::blue);
  txt->setPos(ox-tw - 5, oy);
  txt->setZValue(1);

  txt = addText(QString("%1").arg(_score2), font);
  tw = QFontMetrics(font).width(txt->toPlainText());
  txt->setDefaultTextColor(Qt::red);
  txt->setPos(ox + _width * d, oy);
  txt->setZValue(1);

  QRectF boundingRect(0, 0, d, d);
  for (int y = 0; y < _heigth; ++y) {
    for (int x = 0; x < _width; ++x) {
      boundingRect.moveTopLeft(QPointF(ox + x*d, oy + y*d));
      QPoint p(x, y);
      int index;
      QColor c(255, 255, 0);

      switch (_grid[p]) {
      case Snake1:
        index = _snake1.indexOf(p);
        if (index != -1)
          c = QColor(0, 0, 256 * index / _snake1.size());
        addEllipse(boundingRect, QPen(Qt::black, 0), QBrush(c));
        break;
      case Snake2:
        index = _snake2.indexOf(p);
        if (index != -1)
          c = QColor(256 * index / _snake2.size(), 0, 0);
        addEllipse(boundingRect, QPen(Qt::black, 0), QBrush(c));
        break;
      case Food:
        addEllipse(boundingRect, QPen(Qt::black, 0), QBrush(Qt::NoBrush));
        break;
      default:
        break;
      }
    }
  }
}

void SnakeScene::addRandomFood()
{
  while (true) {
    QPoint random(qrand() % _width, qrand() % _heigth);
    if (_grid[random] == Empty) {
      _grid[random] = Food;
      break;
    }
  }

}
