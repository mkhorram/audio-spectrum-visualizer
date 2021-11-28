#include "mainwindow.h"
#include <QApplication>

//#define UNIT_TEST_ENABLED

#ifndef UNIT_TEST_ENABLED

void setSharedStyleSheet(QApplication &a)
{
    a.setStyleSheet(
                ""
                //"QWidget {\n   border: 1px solid rgb(127, 127, 200);\n   background-color:#2B486E;\n   color:white;\n}\n"
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

#else
#include "tests.hpp"
#endif
