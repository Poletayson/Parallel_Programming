//#include "mainwindow.h"
#include <QCoreApplication>
#include <graphic.h>
#include <iostream>
#include <QImage>
#include <QTime>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "rus");

    Graphic *gr = new Graphic ();
    gr->GetFileImage("Input/in.jpg");      //получаем изображение
    gr->setLIMIT(22);
    QImage* img = gr->outlineSelectionLinear();
    img->save("_out.jpg");
//    int in;
//    MPI_Initialized(&in);
//    if (in)
//        std::cout<<"MPI_Initialized!!!"<<std::endl;
//    QTime st = QTime::currentTime();

//    QTime fn = QTime::currentTime();
//    std::cout<<"Time_Lineaer: "<<st.msecsTo(fn)<<std::endl;
//    std::cout.flush();



//    st = QTime::currentTime();
    img = gr->outlineSelectionMPI(argc, argv);
//    fn = QTime::currentTime();

    //MPI_Finalize();
//    std::cout<<"FINALIZED\n";
//    std::cout.flush();

    //img->save("_out2.jpg");



    //return a.exec();

}
