#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <qlist.h>
#include <QPixelFormat>
#include <QImage>
#include <QPainter>
#include <QtMath>
#include <QRect>
#include <threadgraphic.h>
#include <QDebug>
#include <QPainter>

#include <omp.h>
#include <mpi.h>

class Graphic : public QGraphicsView
{
    Q_OBJECT
public:
   QGraphicsScene* myScene;

   qreal SIGMA;
   QPen *p;


signals:
    void ResizeSignal ();

public:
    QGraphicsPixmapItem *imageItem;
    QGraphicsPixmapItem *reserve;
    int counter = 0;
    explicit Graphic(QWidget *parent = nullptr);
    void clear ();      //очистить
    void ClearItem (QGraphicsItemGroup* Items);
    void GetFileImage (QString file);
                   //отрисовка
    QGraphicsItemGroup* addRect (qreal x, qreal y, qreal w, qreal h, QPen* p, QGraphicsScene* sc);
    void resizeEvent(QResizeEvent *event);
    QPixmap* getNegative ();
    QPixmap* getGray();
    //QPixmap* getNegative ();
    QPixmap* Brightness(int val);

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
    QImage* outlineSelectionParallel (int threadCount);
    QImage* outlineSelectionOMP (int threadCount);
    QImage* outlineSelectionMPI (int threadCount);
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
