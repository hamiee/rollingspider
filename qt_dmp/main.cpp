#include "mainwindow.h"
#include "dialog.h"
#include <QApplication>




int main(int argc, char *argv[])
{
//   QString buf = "d"
//system()
    system("adb forward tcp:38300 tcp:38300");
    QApplication a(argc, argv);
    MainWindow w;


    w.show();
    //Dialog d;
    //d.show();

    return a.exec();
}
