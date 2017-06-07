#include "service.h"
#include <QApplication>
#include <QObject>
#include <time.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Service w;
    qsrand (time(NULL));
    w.show();

    return a.exec();
}
