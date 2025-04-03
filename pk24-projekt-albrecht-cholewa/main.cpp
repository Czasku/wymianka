#include "mainwindow.h"

#include <QApplication>
#include "glowne.h"

#ifdef MAIN

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

#endif
