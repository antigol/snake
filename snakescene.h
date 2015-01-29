#ifndef SNAKESCENE_H
#define SNAKESCENE_H

#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QList>
#include <QMap>
#include <QHash>
#include <QPoint>
#include <QGradient>

class SnakeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SnakeScene(QObject *parent = 0);
    void newGame(int w, int h, int timer);

public slots:
    void pause();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void timerEvent(QTimerEvent *);

private:
    void addRandomFood();

    enum CaseElement {
        Empty,
        Snake1,
        Snake2,
        Food
    };
    enum Direction {
        Up, Dn, Lt, Rt
    };

    int _timerId;
    int _interval;

    int _width;
    int _heigth;

    QList<QPoint> _snake1;
    QList<QPoint> _snake2;

    QList<Direction> _direction1;
    QList<Direction> _direction2;

    Direction _lstDirection1;
    Direction _newDirection1;
    Direction _lstDirection2;
    Direction _newDirection2;

    bool _gameOver1;
    bool _gameOver2;

    int _score1;
    int _score2;

    QHash<QPoint, SnakeScene::CaseElement> _grid;
};

inline uint qHash(QPoint point)
{
    return (point.y() << (sizeof (uint) * 4)) | point.x();
}

#endif // SNAKESCENE_H
