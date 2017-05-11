#ifndef SIMULATOR
#define SIMULATOR

#include <QTimer>
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

class Simulator: public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Simulator(QGraphicsItem * parent=0);
public slots:
    void move();
private:
    //QGraphicsPixmapItem *aircraftIcon;
};

#endif // SIMULATOR

