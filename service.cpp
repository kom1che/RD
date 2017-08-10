#include "service.h"
#include "ui_service.h"
#include "originfpl.h"
#include <QDebug>
#include "sim.h"
#include <qmath.h>
#include "anime.h"
#include <QComboBox>

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
    ui->SpeedSlider->setEnabled(false);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(-20,-20,310,310);
    chart = new QGraphicsPixmapItem();
    chart->setPixmap(QPixmap(":/images/bg_f_Qt.jpg"));
    chart->setPos(-300,-290);
    chart->setScale(0.53);
    scene->addItem(chart);
    ui->Viewer->setScene(scene);
    PAUSE = false;
    times=0;
    keySpeed = checkCopy = checkSub =false;
    syn_pre = true;
    kSpeed = checkAR = 0;
    h=m=0;
    s=1;
    QObject::connect(ui->genBtn, SIGNAL(clicked(bool)), this, SLOT(setFpl()));
    QObject::connect(ui->submitBtn, SIGNAL(clicked(bool)), this, SLOT(RD()));
    QObject::connect(ui->acceptBtn, SIGNAL(clicked(bool)), this, SLOT(Accept()));
    QObject::connect(ui->rejectBtn, SIGNAL(clicked(bool)), this, SLOT(Reject()));
    QObject::connect(ui->runBtn, SIGNAL(clicked(bool)), this, SLOT(move()));
    QObject::connect(ui->pauseBtn, SIGNAL(clicked(bool)), this, SLOT(pause()));
    QObject::connect(ui->checkHigh, SIGNAL(toggled(bool)), this, SLOT(speedUp()));
    QObject::connect(ui->checkLow, SIGNAL(toggled(bool)), this, SLOT(speedLow()));
    QObject::connect(ui->SpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(speedChan(int)));
}

Service::~Service()
{
    delete ui;
}

void Service::setFpl(){
    scene->clear();
    valuex.clear();
    valuey.clear();
    listLine.clear();
    listPoint.clear();
    listLabel.clear();
    chart = new QGraphicsPixmapItem();
    chart->setPixmap(QPixmap(":/images/bg_f_Qt.jpg"));
    chart->setPos(-300,-290);
    chart->setScale(0.53);
    chart->setOpacity(0.35);
    scene->addItem(chart);
    originFpl *FPL = new originFpl();
    anime = new Anime();
    for (size_t i=0; i<12; i++) {
        QString searchName;
        searchName = QString("W")+QString::number(i);
        anime->setScaleX(2.38);
        int valueX = FPL->getxCoor().value(searchName)*anime->getScaleX();
        int valueY = FPL->getyCoor().value(searchName)*anime->getScaleX();
        int valueAlt = FPL->getAlt().value(searchName);
        bool valueMan = FPL->getMandat().value(searchName);
        valuex.append(valueX);
        valuey.append(valueY);
        valuealt.append(valueAlt);
        valuemandat.append(valueMan);
    }
    for (size_t i=0, n=valuex.size(); i<n; i++) {
        pointIcon = new QGraphicsPixmapItem();
        pointIcon->setPixmap(QPixmap(":/images/3ang.png"));
        pointIcon->setScale(0.07);
        pointIcon->setPos(valuex[i], valuey[i]);
        listPoint.append(pointIcon);
        scene->addItem(listPoint[i]);
        QString str;
        for (int j=0; j<3; j++) {
            str[j]=QChar('A' + char(qrand() % ('Z'-'A')));
        }
        label = new QGraphicsTextItem;
        label->setPlainText(str);
        label->setDefaultTextColor(Qt::black);
        label->setFont(QFont("gothic",7));
        label->setPos(valuex[i]+10, valuey[i]-10);
        listLabel.append(label);
        scene->addItem(listLabel[i]);
    }
    for (size_t j=0, m=valuex.size()-1;j<m; j++) {
        line = new QGraphicsLineItem;
        _pen.setColor("lightskyblue");
        _pen.setWidth(2);
        _pen.setStyle(Qt::DashLine);
        line->setPen(_pen);
        line->setLine(valuex[j]+7, valuey[j]+7,valuex[j+1]+7,valuey[j+1]+7);
        listLine.append(line);
        scene->addItem(listLine[j]);
        //scene->addItem(line);
    }
    ui->runBtn->setEnabled(true);
}

void Service::move(){
    anime->setTres(500);
    anime->setXY(valuex, valuey);
    anime->setAlt(valuealt);
    anime->setMandat(valuemandat);
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
    ui->SpeedSlider->setEnabled(true);
}

void Service::copyFpl(QList<int> AA, QList<int> BB, QList<int> CC, QList<bool> DD){
        foreach (int coor, AA) {
            copyx.append(coor);
        }
        foreach (int coor, BB) {
            copyy.append(coor);
        }
        foreach (int alt, CC) {
            copyalt.append(alt);
        }
        foreach (bool man, DD) {
            copymandat.append(man);
        }
//        for (size_t j=0; j<listLine.size()-1; j++) {
//            qDebug() << j <<listLine[j]->line().x1() << listLine[j]->line().x2();
//        }
        //checkCopy = true;
}

void Service::RD(){
    checkAR=1;
//    if (checkCopy == false){
    copyFpl(valuex, valuey, valuealt, valuemandat);
//    } else {
//      //copyFpl(copyx, copyy, copyalt, copymandat);
//    }
    int current = anime->getIter();
    for (size_t j=current+2; j<copyx.size()-4; j++){
        double p = setP(2);
        if (p < 0.25) {
           ui->acceptBtn->setEnabled(true);
           ui->rejectBtn->setEnabled(true);
           ui->submitBtn->setEnabled(false);
           checkSub = true;
           jet = j;
           //qDebug() << "jet" << jet << anime->getIter();
           copyx.removeAt(j);
           copyy.removeAt(j);
           copyalt.removeAt(j);
           copymandat.removeAt(j);
           QPen penalt;
           penalt.setColor(Qt::darkYellow);
           penalt.setWidth(2);
           penalt.setStyle(Qt::DashDotDotLine);
           listLine[j-1]->setPen(penalt);
           listLine[j]->setPen(penalt);
           penalt.setColor(Qt::red);
           penalt.setStyle(Qt::DotLine);
           penalt.setWidth(2.5);
           copyLine = new QGraphicsLineItem;
           copyLine->setPen(penalt);
           copyLine->setLine(copyx[j-1]+7, copyy[j-1]+7, copyx[j]+7, copyy[j]+7);
           //qDebug() << "copyline in RD" <<copyLine->line().x1() << copyLine->line().x2();
           scene->addItem(copyLine);
           break;
        }
    }
    if (checkSub == false) {
        ui->Infolabel->setText("Flight plan has not been modified");
        ui->acceptBtn->setEnabled(false);
        ui->rejectBtn->setEnabled(false);
        if (current<copyx.size()-4){
            checkSub = false;
        }else{
            checkSub = true;
        }
    }   else {
        ui->Infolabel->setText("");
    }
    copyx.clear();
    copyy.clear();
    copyalt.clear();
    copymandat.clear();
}

void Service::Accept(){
    checkAR = 0;
    QPen penalt;
    penalt.setColor("lightskyblue");
    penalt.setWidth(2);
    penalt.setStyle(Qt::DashLine);
    //qDebug() << "size" << listLine.size();
    scene->removeItem(listLine[jet-1]);
    scene->removeItem(listLine[jet]);
    scene->removeItem(listPoint[jet]);
    scene->removeItem(listLabel[jet]);
    scene->removeItem(copyLine);
    //qDebug() << "size before" << valuex.size();
    valuex.removeAt(jet);
    //qDebug() << "size after" << valuex.size();
    valuey.removeAt(jet);
    valuealt.removeAt(jet);
    valuemandat.removeAt(jet);
    anime->removeXY(jet);
    anime->removeALT(jet);
    listPoint.removeAt(jet);
    listLabel.removeAt(jet);
    //qDebug() << "copyline in Accept" <<copyLine->line().x1() << copyLine->line().x2();
    listLine.replace(jet, copyLine);
    listLine.removeAt(jet-1);
    copyLine->setPen(penalt);
    scene->addItem(copyLine);
    ui->submitBtn->setEnabled(true);
    ui->rejectBtn->setEnabled(false);
    ui->acceptBtn->setEnabled(false);
    copyx.clear();
    copyy.clear();
    copyalt.clear();
    copymandat.clear();
    checkSub = false;
    //qDebug() << "size 2" << listLine.size();
}

void Service::Reject(){
    checkAR = 0;
    scene->removeItem(copyLine);
    QPen penalt;
    penalt.setColor("lightskyblue");
    penalt.setWidth(2);
    penalt.setStyle(Qt::DashLine);
    listLine[jet-1]->setPen(penalt);
    listLine[jet]->setPen(penalt);
    copyx.clear();
    copyy.clear();
    copyalt.clear();
    copymandat.clear();
    ui->submitBtn->setEnabled(true);
    ui->rejectBtn->setEnabled(false);
    ui->acceptBtn->setEnabled(false);
    checkSub = false;
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
        ui->checkLow->setEnabled(true);
        anime->setTres(anime->getTres()*2);
        kSpeed=0;
        PAUSE=false;
        anime->clickPause();
        timers->stop();
        keySpeed=false;
        anime->clickResume();
        startTimers();
    }else {
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
    }
}

void Service::speedLow(){
    if (keySpeed==true){
        ui->checkHigh->setEnabled(true);
        anime->setTres(anime->getTres()/2);
        kSpeed=0;
        PAUSE=false;
        anime->clickPause();
        timers->stop();
        keySpeed=false;
        anime->clickResume();
        startTimers();
    }else {
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
    }
}

void Service::comboAct()
{
    if (combo->currentText() == "Feet") {
       anime->setAlt(false);
    } else {
        anime->setAlt(true);
    }
    combo->deleteLater();
}

void Service::speedChan(int v) {
    anime->setStep(double(v)/2);
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
    if (syn_pre == anime->getSyn()){
       ui->timerLabal->setText(str);
       s++;
       if (syn_pre == false){
           syn_pre = true;
       } else {
           syn_pre = false;
       }
    }
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
        ui->genBtn->setEnabled(true);
        ui->pauseBtn->setEnabled(false);
        ui->checkHigh->setEnabled(false);
        ui->checkLow->setEnabled(false);
        ui->checkHigh->setChecked(false);
        ui->checkLow->setChecked(false);
        ui->SpeedSlider->setValue(0);
        ui->SpeedSlider->setEnabled(false);
        h=m=s=0;
        ui->timerLabal->setText(str);
    }
    if (anime->getIter()>=1 && anime->getIter()<anime->getSize()-3 && checkSub == false){
        ui->submitBtn->setEnabled(true);
    }else if (anime->getIter()>anime->getSize()-4 && checkAR == 1){
        ui->rejectBtn->setEnabled(false);
        ui->acceptBtn->setEnabled(false);
        Reject();
    }else {
        ui->submitBtn->setEnabled(false);
    }
    if (anime->getIter() >= jet-1 && jet != 0 && checkSub == true) {
        //qDebug() << "hi" << anime->getIter() << jet;
        Reject();
        ui->submitBtn->setEnabled(true);
        ui->rejectBtn->setEnabled(false);
        ui->acceptBtn->setEnabled(false);
        checkSub = false;
    }
}

double Service::setP(int v){
    double p = 0;
    p = (qrand() % int (qPow(10, v) + 1))/qPow(10, v);
    return p;
}

void Service::mousePressEvent(QMouseEvent *event) {
    if (ui->pauseBtn->isEnabled()) {
        int squarex = qCeil((anime->pos().x()))+72;
        int squarey = qCeil((anime->pos().y()))+64;
        if (event->button() == Qt::RightButton && (event->pos().x() > squarex-10 && event->pos().x() < squarex+10) && (event->pos().y() > squarey-10 && event->pos().y() < squarey+10)){
            combo = new QComboBox();
            combo->addItem("Feet");
            combo->addItem("Meter");
            QFont font ("Century Gothic");
            font.setPointSize(7);
            font.setWeight(QFont::Bold);
            combo->setFont(font);
            combo->setGeometry(anime->pos().x()+15,anime->pos().y()+5,60,25);
            ui->Viewer->scene()->addWidget(combo);
            QObject::connect(combo, SIGNAL(activated(int)), this, SLOT(comboAct()));
        }
    }
}

