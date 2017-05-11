#ifndef SIM
#define SIM

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include "ui_service.h"

class Sim: public QObject, public QGraphicsPixmapItem{
public:
    Sim(QGraphicsItem * parent=0);
public slots:
    void move(QList<int> valx, QList<int> valy);
private:
    QGraphicsPixmapItem *aircraftIcon;
    QTimer * timer;
};

#endif // SIM

