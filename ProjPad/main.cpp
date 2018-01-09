#include "stdafx.h"
#include "ProjPad.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProjPad w;
    w.show();
    return a.exec();
}
