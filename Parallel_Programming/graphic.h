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
//#include <windows.h>
//#define


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
    QPixmap* Contrast (float val);
    QPixmap* Binarization (int val);
    QPixmap* Median ();     //медианный фильтр и Гауссовский
    QPixmap* Gauss ();
    QPixmap* Rezk ();     //медианный фильтр и Гауссовский

    QPixmap* Aquarel ();
    QPixmap* MyFilter (int k);     //Мой фильтр, получившийся из резкости: все цвета за диапазоном - черные

    bool setYUVMatix ();
    bool setYUV ();

    QColor matrixMul (QColor colors[3][3], int matrix[3][3]);
    QColor colorNormir (QColor colorX, QColor colorY);
    bool sobelOperator (QImage image);


    QImage *getImage() const;
    void setImage(QImage *value);

public slots:
    //void Drawing ();
private:
    QImage *image;
    unsigned char *Y, *U, *V;
};

#endif // GRAPHIC_H
