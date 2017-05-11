#include "service.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Service w;
    w.show();

    return a.exec();
}
