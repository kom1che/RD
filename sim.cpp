#include "sim.h"
#include <QTimer>
#include "ui_service.h"

Sim::Sim(QGraphicsItem *parent){
    aircraftIcon = new QGraphicsPixmapItem();
    aircraftIcon->setPixmap(QPixmap(":/images/aircraft.png"));
    aircraftIcon->setScale(0.1);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(50000);

}

void Sim::move(QList<int> valx, QList<int> valy){
    //ui->setText(QString::number(12));
    //ui.
}
