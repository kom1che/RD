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
    void setXY(QList<int> XX, QList<int> YY);

public slots:
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
    int getSize();
    int getStep();
    void setStep(int v);
    int getTres();
    void setTres(int v);
    double getTime();
private:
    double X;
    bool key;
    double Y;
    int step;
    int Tres;
    int iter;
    double time;
    double angle;
    double an;
    double ang;
    double deg;
    QList<int> xx;
    QList<int> yy;
    QList<double> l;
    bool check;
    QTimer *move;
    double dyy;
    double dxx;
    QPainter p;
    bool pause;
    bool stop;
    bool syn;
    double pauseIT;
    double pauseX;
    double pauseY;
    double pauseAn;
};

#endif // ANIME

