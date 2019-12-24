#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <qlist.h>
#include <QPixelFormat>
#include <QImage>
#include <QFile>
#include <QPainter>
#include <QtMath>
#include <QRect>
//#include <threadgraphic.h>
#include <QDebug>
#include <QPainter>
#include <iostream>

#include <omp.h>
#include <mpi.h>

class Graphic : public QObject
{
    Q_OBJECT
public:
   qreal SIGMA;
   QPen *p;


public:

    int counter = 0;
    explicit Graphic(QObject *parent = nullptr);
    void clear ();      //очистить

    void GetFileImage (QString file);
                   //отрисовка

    bool setYUVMatix ();
    bool setYUV ();
    void sobelOperator ();
    void Binarization ();

    bool setYUVMatixOMP ();
    bool setYUVOMP ();
    void sobelOperatorOMP ();
    void BinarizationOMP ();

    bool setYUVMatixMPI ();
    bool setYUVMPI ();
    void sobelOperatorMPI ();
    void BinarizationMPI ();


    QColor matrixColorMul (QColor colors[3][3], int matrix[3][3]);
    QRgb matrixColorMul (QRgb colors[3][3], int matrix[3][3]);
    unsigned char matrixMul (unsigned char channel[3][3], int matrix[3][3]);
    QColor colorNormir (QColor colorX, QColor colorY);
    QRgb colorNormir (QRgb colorX, QRgb colorY);
    unsigned char componentNormir (unsigned char colorX, unsigned char colorY);



    QImage* outlineSelectionLinear ();
    QImage* outlineSelectionOMP (int threadCount);
    QImage* outlineSelectionMPI (int argc, char *argv[]);
    QImage* sobelOperatorOneChannel (unsigned char *matix);


    QImage *getImage() const;
    void setImage(QImage *value);

    unsigned char *getY() const;

    unsigned char *getU() const;

    unsigned char *getV() const;

    void setLIMIT(int value);

public slots:
    //void Drawing ();
private:
    int LIMIT = 225;

    QImage *image;
    int width;
    int height;
    unsigned char *Y, *U, *V;
};

#endif // GRAPHIC_H
