//#include "mainwindow.h"
//#include <QCoreApplication>
#include <graphic.h>
#include <iostream>
#include <QImage>
#include <QTime>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "rus");
    //std::cout<<"Args: "<<argv[0]<<argv[1]<<argv[2]<<argv[3];


    Graphic *gr = new Graphic ();
    gr->GetFileImage("C:/Qt/Projects/Parallel_Programming/build-Parallel_Programming_MPI_Console-Desktop_Qt_5_11_1_MinGW_32bit_Static-Release/release/Input/scale_1200.webp");      //получаем изображение
    gr->setLIMIT(25);

    QTime st = QTime::currentTime();
    QImage* img = gr->outlineSelectionLinear();
    QTime fn = QTime::currentTime();
    std::cout<<"Time_Lineaer: "<<st.msecsTo(fn)<<std::endl;
    std::cout.flush();
    img->save("Input/_out.jpg");


    st = QTime::currentTime();
    img = gr->outlineSelectionMPI(argc, argv);
    fn = QTime::currentTime();
    std::cout<<"Time_MPI: "<<st.msecsTo(fn)<<std::endl;
    //img->save("Input/_out.jpg");

    //return a.exec();

}
