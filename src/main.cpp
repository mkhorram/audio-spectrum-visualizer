#include "mainwindow.h"
#include <QApplication>



void setSharedStyleSheet(QApplication &a)
{
    a.setStyleSheet(
                ""
                );
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setSharedStyleSheet(a);

    MainWindow w;
    w.show();

    return a.exec();
}
