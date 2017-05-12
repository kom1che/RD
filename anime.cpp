#include "anime.h"
#include <QPixmap>
#include <QTimer>
#include <qmath.h>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include "service.h"


Anime::Anime(QGraphicsPathItem *parent) {
    setPixmap(QPixmap(":/images/aircraft_1_1.png"));
    setScale(0.03);
    check = true;
    pause = false;
    stop = false;
    syn = false;
    iter=0;
    step=4;
    Tres=1000;
    time=0;
    key=false;
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
            if (Y < yy[iter+1] && X < xx[iter+1]){
                setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
            } else if (an < 0 && Y > yy[iter+1] && X < xx[iter+1]) {
                setPos(X+7-(qCos(deg+angle)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(deg+angle)*size.height()*0.03/2*qSqrt(2)));
            }else if (an == 0 && Y == yy[iter+1] && X < xx[iter+1]){
                setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
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
                    setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
                }
            }
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

void Anime::setPause(bool v){
    pause = v;
}

bool Anime::getPause(){
return pause;
}

bool Anime::getSyn(){
return syn;
}

void Anime::setStep(int v){
    step = v;
}

int Anime::getStep(){
return step;
}

void Anime::setTres(int v){
    Tres = v;
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
                setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
            } else if (an < 0 && Y > yy[iter+1] && X < xx[iter+1]) {
                setPos(X+7-(qCos(deg+angle)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(deg+angle)*size.height()*0.03/2*qSqrt(2)));
            }else if (an == 0 && Y == yy[iter+1] && X < xx[iter+1]){
                setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
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
                    setPos(X+7-(qCos(angle+deg)*size.width()*0.03/2*qSqrt(2)),Y+7-(qSin(angle+deg)*size.height()*0.03/2*qSqrt(2)));
                }
            }
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
