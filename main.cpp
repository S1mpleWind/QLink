#include "MainWindow.h"
//#include "GameMap.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //GameMap m;
    //m.show();
    return a.exec();
}
