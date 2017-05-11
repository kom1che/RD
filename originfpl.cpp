#include "originfpl.h"
#include <QtGlobal>

originFpl::originFpl(){
    Mandatory = true;
    for (size_t i=0; i<12; i++){
        if (i==0) {
            X = 0;
            Y = 0;
            AltM = 0;
            Name = "W0";
            xCoor.insert(Name, X);
            yCoor.insert(Name, Y);
            Alt.insert(Name, AltM);
            Mandat.insert(Name, Mandatory);
        } else if(i==1) {
            X = 10;
            Y = 10;
            AltM = 10000;
            Name = "W1";
            xCoor.insert(Name, X);
            yCoor.insert(Name, Y);
            Alt.insert(Name, AltM);
            Mandat.insert(Name, Mandatory);
        } else if(i==10) {
            X = 100;
            Y = 100;
            AltM = 10000;
            Name = "W10";
            xCoor.insert(Name, X);
            yCoor.insert(Name, Y);
            Alt.insert(Name, AltM);
            Mandat.insert(Name, Mandatory);
        } else if(i==11) {
            X = 110;
            Y = 110;
            AltM = 0;
            Name = "W11";
            xCoor.insert(Name, X);
            yCoor.insert(Name, Y);
            Alt.insert(Name, AltM);
            Mandat.insert(Name, Mandatory);
        } else {
            X = i*10;
            Y = X+(-2+(qrand() % 16));
            AltM = 10000+(-500+(qrand() % 1000));
            Name = QString("W")+QString::number(i);
            xCoor.insert(Name, X);
            yCoor.insert(Name, Y);
            Alt.insert(Name, AltM);
            Mandat.insert(Name, Mandatory);
        }
    }
}

QMap<QString, int> originFpl::getxCoor(){
    return xCoor;
}

QMap<QString, int> originFpl::getyCoor(){
return yCoor;
}

QMap<QString, int> originFpl::getAlt(){
return Alt;
}

QMap<QString, bool> originFpl::getMandat(){
    return Mandat;
}
