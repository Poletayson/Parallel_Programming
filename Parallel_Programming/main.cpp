#include "mainwindow.h"
#include <QApplication>
#include <iostream>



int main(int argc, char *argv[])
{

    setlocale(LC_ALL, "rus");
    //std::cout<<"Args: "<<argv[0]<<argv[1]<<argv[2]<<argv[3];


    MPI_Init(&argc, &argv);
    int procRank, procSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);   //получаем номер процесса
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);

    qDebug()<<"Threads: "<<procSize;
    //MPI_Init(nullptr, nullptr);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    MPI_Finalize();
}
