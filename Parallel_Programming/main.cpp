#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
//    qDebug()<<"argc "<<argc;
    MPI_Init(&argc, &argv);

    //MPI_Init(nullptr, nullptr);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    MPI_Finalize();
}
