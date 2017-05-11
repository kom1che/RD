#ifndef SERVICE_H
#define SERVICE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "ui_service.h"
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QTime>
#include <QObject>
class Anime;

namespace Ui {
class Service;
}

class Service : public QWidget
{
    Q_OBJECT

public:
    explicit Service(QWidget *parent = 0);
    ~Service();
public slots:
    void setFpl();
    void setSim();
    void move();
    void pause();
    void speedUp();
    void speedLow();
    void resume();
    void setPause(bool v);
    void startTimers();
    void startCount();
private:
    Ui::Service *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *pointIcon;
    QGraphicsPixmapItem *chart;
    QGraphicsPixmapItem *aircraftIcon;
    QGraphicsLineItem *line;
    QList<int> valuex;
    QList<int> valuey;
    QTimer *timer;
    QTime time;
    QTimer *timers;
    QTime timeMove;
    double dx;
    int step;
    int Tres;
    int h,m,s,t,kSpeed;
    double dy;
    bool keySpeed;
    bool PAUSE;
    bool syn_pre;
    double times;
    QGraphicsTextItem *label;
    Anime *anime;
};

#endif // SERVICE_H
