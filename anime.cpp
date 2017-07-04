#include "anime.h"
#include <QPixmap>
#include <QTimer>
#include <qmath.h>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include "service.h"
//Hi! Its me on 04 July 2017!!!


Anime::Anime(QGraphicsPathItem *parent) {
    setPixmap(QPixmap(":/images/aircraft_1_1.png"));
    setScale(0.03);
    check = true;
    pause = false;
    stop = false;
    syn = false;
    checkAlt = true;
    iter=0;
    step=0.5;
    Tres=1000;
    time=0;
    key=false;
    check_first_iter = true;
    X = Y = 0;
    label2 = new QGraphicsTextItem;
    label2->setDefaultTextColor("purple");
    label2->setFont(QFont("gothic",7.5));
    label3 = new QGraphicsTextItem;
    label3->setDefaultTextColor("purple");
    label3->setFont(QFont("gothic",7.5));
    label4 = new QGraphicsTextItem;
    label4->setDefaultTextColor("purple");
    label4->setFont(QFont("gothic",7.5));
}

void Anime:: startTimer(){
    move = new QTimer(this);
    connect(move, SIGNAL(timeout()), this, SLOT(startMove()));
    move->start(Tres);
}

void Anime:: setXY(QList<int> XX, QList<int> YY){
    foreach (int coor, XX) {
        xx.append(coor);
    }
    foreach (int coor, YY) {
        yy.append(coor);
    }
    for (size_t i=1; i<xx.size(); i++) {
        l.append(sqrt(qPow(xx.at(i)-xx.at(i-1),2)+qPow(yy.at(i)-yy.at(i-1),2)));
        time = time + (sqrt(qPow(xx.at(i)-xx.at(i-1),2)+qPow(yy.at(i)-yy.at(i-1),2))/step*Tres);
    }
}

void Anime:: setAlt(QList<int> ALT){
    foreach (int alt, ALT) {
        altitude.append(alt);
    }
}

void Anime:: setMandat(QList<bool> MANDAT){
    foreach (bool man, MANDAT) {
        mandat.append(man);
    }
}

void Anime::startMove() {
    QSize size = QPixmap(":/images/aircraft_1_1.png").size();
    if (iter < xx.size()-1){
            double dx = xx[iter+1]-xx[iter];
            double dy = yy[iter+1]-yy[iter];
            double angle = qAtan(dy/dx);
            double an = qRadiansToDegrees(angle);
            dyy = step * qSin(angle);
            dxx = step * qCos(angle);
            setRotation(an);
            X=X+dxx;
            Y=Y+dyy;
            double deg = qDegreesToRadians(45.0);
            line2 = new QGraphicsLineItem;
            QPen _pen;
            _pen.setColor("darkblue");
            _pen.setWidth(2);
            _pen.setStyle(Qt::SolidLine);
            line2->setPen(_pen);
            if (check_first_iter) {
                position_x = X+7;
                position_y = Y+7;
                check_first_iter = false;
            } else {
                line2->setLine(position_x, position_y,X+7,Y+7);
                scene()->addItem(line2);
                label2->setPlainText("current x: "+QString::number((position_x-7)/SCALEX, 'g', 3));
                label2->setPos(position_x-70, position_y+4);
                label3->setPlainText("current y: "+QString::number((position_y-7)/SCALEX, 'g', 3));
                label3->setPos(position_x-70, position_y+14);
                label4->setPos(position_x-70, position_y+24);
                double deltaxn = qFabs(((position_x-7)/SCALEX)-(qCeil(xx[iter]/SCALEX)))/qFabs(qCeil(xx[iter+1]/SCALEX)-qCeil(xx[iter]/SCALEX));
                int hn = ((altitude[iter+1]-altitude[iter])*deltaxn)+altitude[iter];
                if (checkAlt) {
                    label4->setPlainText("altitude: "+QString::number(hn)+" m");
                } else {
                    label4->setPlainText("altitude: FL "+QString::number(qCeil(hn*3.2808399/100)));
                }
                position_x = X+7;
                position_y = Y+7;
            }
            if (Y < yy[iter+1] && X < xx[iter+1]){
                current_x=X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2));
                current_y=Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2));
                //setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
            } else if (an < 0 && Y > yy[iter+1] && X < xx[iter+1]) {
                current_x=X+7-(qCos(deg+angle)*size.width()*0.03/2*qSqrt(2));
                current_y=Y+7-(qSin(deg+angle)*size.height()*0.03/2*qSqrt(2));
                //setPos(X+7-(qCos(deg+angle)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(deg+angle)*size.height()*0.03/2*qSqrt(2)));
            }else if (an == 0 && Y == yy[iter+1] && X < xx[iter+1]){
                current_x=X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2));
                current_y=Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2));
                //setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
            }else{
                iter++;
                if (iter < xx.size()-1) {
                    double dxout=X;
                    double dyout=Y;
                    double stepout = sqrt(qPow(dxout-X,2)+qPow(dyout-Y,2));
                    X = xx[iter];
                    Y = yy[iter];
                    dx = xx[iter+1]-xx[iter];
                    dy = yy[iter+1]-yy[iter];
                    angle = qAtan(dy/dx);
                    an = qRadiansToDegrees(angle);
                    dyy = stepout * qSin(angle);
                    dxx = stepout * qCos(angle);
                    setRotation(an);
                    X=X+dxx;
                    Y=Y+dyy;
                    current_x=X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2));
                    current_y=Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2));
                    //setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
                }
            }
          setPos(current_x,current_y);
          if (syn == true && check_speed == false){
              syn = false;
          } else if (syn == false && check_speed == false){
              syn = true;
          }
    } else {
         label4->setPlainText("altitude: "+QString::number(altitude[iter]));
         move->stop();
         setStop(true);
         key=true;
    }
     //qDebug() << "syn" <<  syn;
}

void Anime::setPause(bool v){
    pause = v;
}

void Anime::setScaleX(double v){
    SCALEX = v;
}

void Anime::removeXY(int v){
    xx.removeAt(v);
    yy.removeAt(v);
}

void Anime::removeALT(int v){
    altitude.removeAt(v);
}

bool Anime::getPause(){
return pause;
}

bool Anime::getSyn(){
return syn;
}

void Anime::setStep(double v){
    step = v;
}

double Anime::getStep(){
return step;
}

void Anime::setTres(int v){
    Tres = v;
}

void Anime::setAlt(bool v)
{
    checkAlt = v;
}

double Anime::getScaleX(){
return SCALEX;
}

void Anime::setCheckSpeed(bool v){
    check_speed = v;
}

int Anime::getTres(){
return Tres;
}

int Anime::getIter(){
return iter;
}

int Anime::getSize(){
return xx.size()-1;
}

double Anime::getTime(){
return time;
}

bool Anime::getKey(){
return key;
}

void Anime::setStop(bool v){
    stop = v;
}

bool Anime::getStop(){
return stop;
}

void Anime::clickPause(){
    move->stop();
    pauseIT=iter;
    pauseX=X;
    pauseY=Y;
    pauseAn=an;
    pause=true;
    //qDebug() << "Pause clikced";
}

void Anime::clickResume(){
    move->start(Tres);
    QSize size = QPixmap(":/images/aircraft_1_1.png").size();
    //qDebug() << "Resume clikced";
    if (pause) {
        iter=pauseIT;
        X=pauseX;
        Y=pauseY;
        pause=false;
    }
    if (iter < xx.size()-1){
            double dx = xx[iter+1]-xx[iter];
            double dy = yy[iter+1]-yy[iter];
            double angle = qAtan(dy/dx);
            double an = qRadiansToDegrees(angle);
            dyy = step * qSin(angle);
            dxx = step * qCos(angle);
            setRotation(an);
            X=X+dxx;
            Y=Y+dyy;
            double deg = qDegreesToRadians(45.0);
            if (Y < yy[iter+1] && X < xx[iter+1]){
                current_x=X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2));
                current_y=Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2));
                //setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
            } else if (an < 0 && Y > yy[iter+1] && X < xx[iter+1]) {
                current_x=X+7-(qCos(deg+angle)*size.width()*0.03/2*qSqrt(2));
                current_y=Y+7-(qSin(deg+angle)*size.height()*0.03/2*qSqrt(2));
                //setPos(X+7-(qCos(deg+angle)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(deg+angle)*size.height()*0.03/2*qSqrt(2)));
            }else if (an == 0 && Y == yy[iter+1] && X < xx[iter+1]){
                current_x=X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2));
                current_y=Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2));
                //setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
            }else{
                iter++;
                if (iter < xx.size()-1) {
                    double dxout=X;
                    double dyout=Y;
                    double stepout = sqrt(qPow(dxout-X,2)+qPow(dyout-Y,2));
                    X = xx[iter];
                    Y = yy[iter];
                    dx = xx[iter+1]-xx[iter];
                    dy = yy[iter+1]-yy[iter];
                    angle = qAtan(dy/dx);
                    an = qRadiansToDegrees(angle);
                    dyy = stepout * qSin(angle);
                    dxx = stepout * qCos(angle);
                    setRotation(an);
                    X=X+dxx;
                    Y=Y+dyy;
                    current_x=X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2));
                    current_y=Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2));
                    //setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
                }
            }
          setPos(current_x,current_y);
            if (syn == true && check_speed == false){
                syn = false;
            } else if (syn == false && check_speed == false){
                syn = true;
            }
    } else {
         move->stop();
         setStop(true);
         key=true;
    }
     //qDebug() << "syn" <<  syn;
}
