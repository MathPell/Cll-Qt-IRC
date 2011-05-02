#include <QtGui/QApplication>
#include "mainclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainClient w;
    w.show();

    return a.exec();
}
