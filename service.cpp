#include "service.h"
#include "ui_service.h"
#include "originfpl.h"
#include <QDebug>
#include "sim.h"
#include <qmath.h>
#include "anime.h"

Service::Service(QWidget *parent) :
    QWidget(parent), ui(new Ui::Service){
    ui->setupUi(this);
    ui->acceptBtn->setEnabled(false);
    ui->checkLow->setEnabled(false);
    ui->checkHigh->setEnabled(false);
    ui->pauseBtn->setEnabled(false);
    ui->rejectBtn->setEnabled(false);
    ui->submitBtn->setEnabled(false);
    ui->runBtn->setEnabled(false);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(-20,-20,310,310);
    chart = new QGraphicsPixmapItem();
    chart->setPixmap(QPixmap(":/images/chart.png"));
    chart->setPos(-60,-110);
    chart->setScale(0.24);
    scene->addItem(chart);
    ui->Viewer->setScene(scene);
    PAUSE = false;
    times=0;
    keySpeed=false;
    syn_pre = true;
    kSpeed=0;
    h=m=0;
    s=1;
    QObject::connect(ui->genBtn, SIGNAL(clicked(bool)), this, SLOT(setFpl()));
    QObject::connect(ui->runBtn, SIGNAL(clicked(bool)), this, SLOT(move()));
    QObject::connect(ui->pauseBtn, SIGNAL(clicked(bool)), this, SLOT(pause()));
    QObject::connect(ui->checkHigh, SIGNAL(toggled(bool)), this, SLOT(speedUp()));
    QObject::connect(ui->checkLow, SIGNAL(toggled(bool)), this, SLOT(speedLow()));
}

Service::~Service()
{
    delete ui;
}

void Service::setFpl(){
    scene->clear();
    valuex.clear();
    valuey.clear();
    chart = new QGraphicsPixmapItem();
    chart->setPixmap(QPixmap(":/images/chart.png"));
    chart->setPos(-60,-110);
    chart->setScale(0.24);
    chart->setOpacity(0.55);
    scene->addItem(chart);
    originFpl *FPL = new originFpl();
    for (size_t i=0; i<12; i++) {
        QString searchName;
        searchName = QString("W")+QString::number(i);
        double SCALE = 2.38;
        int valueX = FPL->getxCoor().value(searchName)*SCALE;
        int valueY = FPL->getyCoor().value(searchName)*SCALE;
        valuex.append(valueX);
        valuey.append(valueY);
    }
    for (size_t i=0, n=valuex.size(); i<n; i++) {
        pointIcon = new QGraphicsPixmapItem();
        pointIcon->setPixmap(QPixmap(":/images/3ang.png"));
        pointIcon->setScale(0.07);
        pointIcon->setPos(valuex[i], valuey[i]);
        scene->addItem(pointIcon);
        QString str;
        for (int j=0; j<3; j++) {
            str[j]=QChar('A' + char(qrand() % ('Z'-'A')));
        }
        label = new QGraphicsTextItem;
        label->setPlainText(str);
        label->setDefaultTextColor(Qt::black);
        label->setFont(QFont("gothic",7));
        label->setPos(valuex[i]+10, valuey[i]-10);
        scene->addItem(label);
    }
    for (size_t j=0, m=valuex.size()-1;j<m; j++) {
        line = new QGraphicsLineItem;
        QPen _pen;
        _pen.setColor("lightskyblue");
        _pen.setWidth(2);
        _pen.setStyle(Qt::DashLine);
        line->setPen(_pen);
        line->setLine(valuex[j]+7, valuey[j]+7,valuex[j+1]+7,valuey[j+1]+7);
        scene->addItem(line);
    }
    ui->runBtn->setEnabled(true);
    //QObject::connect(ui->runBtn, SIGNAL(clicked(bool)), this, SLOT(setSim()));
}

void Service::setSim(){
    ui->pauseBtn->setEnabled(true);
    ui->genBtn->setEnabled(false);
    ui->checkHigh->setEnabled(true);
    ui->checkLow->setEnabled(true);
    ui->runBtn->setEnabled(false);
}

void Service::move(){
    anime = new Anime();
    anime->setStep(1);
    anime->setTres(500);
    anime->setXY(valuex, valuey);
    anime->startTimer();
    anime->startMove();
    startTimers();
    scene->addItem(anime);
    scene->addItem(anime->label2);
    scene->addItem(anime->label3);
    scene->addItem(anime->label4);
    ui->runBtn->setEnabled(false);
    ui->pauseBtn->setEnabled(true);
    ui->genBtn->setEnabled(false);
    ui->checkHigh->setEnabled(true);
    ui->checkLow->setEnabled(true);
}

void Service::pause(){
    if (PAUSE == false) {
        anime->clickPause();
        timers->stop();
        PAUSE=true;
        ui->checkHigh->setEnabled(false);
        ui->checkLow->setEnabled(false);
    } else {
        anime->clickResume();
        startTimers();
        PAUSE=false;
        if (ui->checkHigh->isChecked()){
            ui->checkHigh->setEnabled(true);
            ui->checkLow->setEnabled(false);
        } else if (ui->checkLow->isChecked()) {
        ui->checkHigh->setEnabled(false);
        ui->checkLow->setEnabled(true);
        } else {
            ui->checkHigh->setEnabled(true);
            ui->checkLow->setEnabled(true);
        }
    }
}

void Service::speedUp(){
    if (keySpeed==true){
//        qDebug() << "keySpeed" << keySpeed;
//        qDebug() << "Tres" << anime->getTres();
        ui->checkLow->setEnabled(true);
        anime->setTres(anime->getTres()*2);
        kSpeed=0;
        PAUSE=false;
        anime->clickPause();
        timers->stop();
        keySpeed=false;
        anime->clickResume();
        startTimers();
//        qDebug() << "keySpeed" << keySpeed;
//        qDebug() << "Tres" << anime->getTres();
    }else {
//        qDebug() << "keySpeed" << keySpeed;
//        qDebug() << "Tres" << anime->getTres();
        anime->setCheckSpeed(true);
        ui->checkLow->setEnabled(false);
        anime->setTres(anime->getTres()/2);
        kSpeed=1;
        PAUSE=false;
        anime->clickPause();
        timers->stop();
        keySpeed=true;
        anime->clickResume();
        startTimers();
        anime->setCheckSpeed(false);
//        qDebug() << "keySpeed" << keySpeed;
//        qDebug() << "Tres" << anime->getTres();
    }
}

void Service::speedLow(){
    if (keySpeed==true){
//        qDebug() << "keySpeed" << keySpeed;
//        qDebug() << "Tres" << anime->getTres();
        ui->checkHigh->setEnabled(true);
        anime->setTres(anime->getTres()/2);
        kSpeed=0;
        PAUSE=false;
        anime->clickPause();
        timers->stop();
        keySpeed=false;
        anime->clickResume();
        startTimers();
//        qDebug() << "keySpeed" << keySpeed;
//        qDebug() << "Tres" << anime->getTres();
    }else {
//        qDebug() << "keySpeed" << keySpeed;
//        qDebug() << "Tres" << anime->getTres();
        anime->setCheckSpeed(true);
        ui->checkHigh->setEnabled(false);
        anime->setTres(anime->getTres()*2);
        kSpeed=2;
        PAUSE=false;
        anime->clickPause();
        timers->stop();
        keySpeed=true;
        anime->clickResume();
        startTimers();
        anime->setCheckSpeed(false);
//        qDebug() << "keySpeed" << keySpeed;
//        qDebug() << "Tres" << anime->getTres();
    }
}

void Service::resume(){
    timer->start();

}

void Service::setPause(bool v){
    PAUSE = v;
}

void Service:: startTimers(){
    timers = new QTimer(this);
    connect(timers, SIGNAL(timeout()), this, SLOT(startCount()));
    timers->start();
    t=anime->getTres();
}

void Service:: startCount(){
    QTime time (h,m,s);
    QString str;
    str=time.toString("hh:mm:ss");
    //qDebug() << "syn_pre" << syn_pre;
    if (syn_pre == anime->getSyn()){
       ui->timerLabal->setText(str);
       //qDebug() << str;
       s++;
       if (syn_pre == false){
           syn_pre = true;
       } else {
           syn_pre = false;
       }
    }
    //s++;
    if (kSpeed==0){
       timers->start(anime->getTres());
    } else if (kSpeed==1){
       timers->start(anime->getTres());
    }else {
       timers->start(anime->getTres());
    }
    if (s>59){
        m++;
        s=0;
    }
    if (anime->getKey() == true){
        timers->stop();
        //qDebug() << "Service Timer stops";
        ui->genBtn->setEnabled(true);
        ui->pauseBtn->setEnabled(false);
        ui->checkHigh->setEnabled(false);
        ui->checkLow->setEnabled(false);
    }
}

