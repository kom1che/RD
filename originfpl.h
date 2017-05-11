#ifndef ORIGINFPL
#define ORIGINFPL

#include <QObject>
#include <QMap>

class originFpl: public QObject {
    Q_OBJECT
public:
    originFpl();
    QMap<QString, int> getxCoor();
    QMap<QString, int> getyCoor();
    QMap<QString, int> getAlt();
    QMap<QString, bool> getMandat();
private:
    QMap<QString, int> xCoor;
    QMap<QString, int> yCoor;
    QMap<QString, int> Alt;
    QMap<QString, bool> Mandat;
    int X;
    int Y;
    int AltM;
    bool Mandatory;
    QString Name;
};

#endif // ORIGINFPL

