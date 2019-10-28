#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QApplication>
#include <QFileDialog>
#include <graphic.h>
#include <iterator>
#include <qmath.h>
//#include <windows.h>
#include <QWidget>
#include <QGraphicsItem>
#include <QPixelFormat>
#include <QImage>
#include <QRandomGenerator>
#include <math.h>
#include <QTimer>
#include <QTime>
#include <QSlider>
#include <QButtonGroup>
#include <dialog.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Graphic* myGraphic;
    Graphic* myGraphic2;
    Graphic* histogram;
    QTimer *Timer;
//    ITunable *car;

//    Car* Cars;
//    Car* CarsForSale;
    QGraphicsItemGroup *carIt;
    QGraphicsItem *box;

    QTimer *timer;
    QRandomGenerator* random;
    QButtonGroup* buttGr;
    int buttNum;


    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

public slots:
    void setStart ();
    void buttonChange ();
    void sliderOb ();
//    void Brightness ();
//    void Binarization ();
//    void Gray ();
//    void Negative ();
//    void Contrast ();
//    void Balance ();

    void makeHist ();

    void Repair ();

    void setNoise ();
    void Median ();
    void Gauss ();
//    void Rezk();
//    void Aquarel();

//    void MyFilter();

    bool setYUVMatix ();
    bool setYUV ();
//            bool setYUV ();
//                bool setYUV ();

    QString getFile();



private slots:
    void on_pushButtonYUV_clicked();

    void on_ButtonAquarel_clicked();

    void on_ButtonMy_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    int m = 10;             //величина сдвига
    float u = M_PI/20;
    float scale = 1.2;
    bool proc = false;

    QImage *image;
    unsigned char *Y, *U, *V;

};

#endif // MAINWINDOW_H
