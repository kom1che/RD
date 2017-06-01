#ifndef ANIME
#define ANIME

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>
class Anime;

class Anime: public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Anime(QGraphicsPathItem * parent=0);
    QGraphicsTextItem *label2;
    QGraphicsTextItem *label3;
    QGraphicsTextItem *label4;

public slots:
    void setXY(QList<int> XX, QList<int> YY);
    void setAlt(QList<int> ALT);
    void setMandat(QList<bool> MANDAT);
    void startTimer();
    void startMove();
    void setPause(bool v);
    bool getPause();
    bool getKey();
    bool getSyn();
    void setStop(bool v);
    bool getStop();
    void clickPause();
    void clickResume();
    int getIter();
    void setCheckSpeed(bool v);
    int getSize();
    double getStep();
    void setStep(double v);
    int getTres();
    void setTres(int v);
    double getTime();
    void setScaleX(double v);
    double getScaleX();
private:
    QGraphicsLineItem *line2;
    double X;
    bool key;
    double Y;
    double step;
    int Tres;
    int iter;
    double time;
    double angle;
    double an;
    double ang;
    double deg;
    QList<int> xx;
    QList<int> yy;
    QList<int> altitude;
    QList<bool> mandat;
    QList<double> l;
    bool check;
    QTimer *move;
    double dyy;
    double dxx;
    QPainter p;
    bool pause;
    bool stop;
    bool syn;
    bool check_speed;
    double pauseIT;
    double pauseX;
    double pauseY;
    double pauseAn;
    double current_x, current_y;
    double position_x, position_y;
    bool check_first_iter;
    double SCALEX;
};

#endif // ANIME

