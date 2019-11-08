#ifndef THREADGRAPHIC_H
#define THREADGRAPHIC_H

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
#include <QThread>

class ThreadGraphic : public QThread
{
    Q_OBJECT
public:
    explicit ThreadGraphic(QObject *parent = nullptr);
    ~ThreadGraphic() override;

    void setImagePointer(QImage *value);

    void setYUVPointers(unsigned char *Yp, unsigned char *Up, unsigned char *Vp);

    void setRect(const QRect &value);


    void run() override;
    void setLIMIT(int value);

    QImage *getImagePart() const;

signals:

public slots:

private:
    int LIMIT = 225;
    int w;

    QImage *imagePart;
    QImage *image;
    QImage *imageReserv;
    unsigned char *Y, *U, *V;
    QRect rect;     //прямоугольник, который будет обрабатываться потоком

    QColor matrixColorMul (QColor colors[3][3], int matrix[3][3]);
    QRgb matrixColorMul (QRgb colors[3][3], int matrix[3][3]);
    unsigned char matrixMul (unsigned char channel[3][3], int matrix[3][3]);
    QColor colorNormir (QColor colorX, QColor colorY);
    QRgb colorNormir (QRgb colorX, QRgb colorY);
    unsigned char componentNormir (unsigned char colorX, unsigned char colorY);

    bool setYUVMatix ();
    bool setYUV ();
    void sobelOperator();       ///ДОДЕЛАТЬ!!!
    void binarization ();

    bool setYUVMatixPart();
    bool setYUVPart();
    void sobelOperatorPart();
    void binarizationPart();
};

#endif // THREADGRAPHIC_H
