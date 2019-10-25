#include "graphic.h"

Graphic::Graphic(QWidget *parent) : QGraphicsView(parent)
{
//    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setAlignment(Qt::AlignCenter);        //выравнивание по центру
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    imageItem = nullptr;
    reserve = nullptr;
    myScene = new QGraphicsScene ();
    this->setScene(myScene);

    SIGMA = 0.8;
    //connect(this, SIGNAL (ResizeSignal()), this, SLOT(Drawing()));
}



void Graphic::ClearItem (QGraphicsItemGroup* Items)
{
    foreach (QGraphicsItem* items, myScene->items(Items->boundingRect()))
        if (items->group() == Items)
            delete items;
}

void Graphic::GetFileImage (QString file)
{
    if (!imageItem)
        delete (imageItem);
    image = new QImage (file);                        //картинка
    imageItem = myScene->addPixmap(QPixmap::fromImage(*image));   //добавляем пиксмап и получаем итем
    myScene->setSceneRect(0, 0, image->width(), image->height());   //размер сцены под картинку
}

QPixmap* Graphic::getNegative ()           //негатив
{
    QImage newImage;
    if (imageItem != nullptr)
    {
        newImage = *new QImage (imageItem->pixmap().toImage());
        //newImage.invertPixels();
        QColor* col = new QColor ();
        for (int i = 0; i < newImage.width(); i++)
            for (int j = 0; j < newImage.height(); j++)
            {
                delete col;
                col = new QColor (newImage.pixelColor(i, j));
                col->setBlue(255 - col->blue());
                col->setGreen(255 - col->green());
                col->setRed(255 - col->red());
                newImage.setPixelColor(i, j, *col);
            }
        delete col;
    }
    return new QPixmap (QPixmap::fromImage(newImage));
}

QPixmap* Graphic::getGray()
{
    QImage newImage;
    if (imageItem != nullptr)
    {
        newImage = *new QImage (imageItem->pixmap().toImage());
        qreal bright;
        QColor* col = new QColor ();
        for (int i = 0; i < newImage.width(); i++)
            for (int j = 0; j < newImage.height(); j++)
            {
                delete col;
                col = new QColor (newImage.pixelColor(i, j));
                //Y=0,299R + 0,5876G + 0,114B
                bright = 0.299*col->red() + 0.5876*col->green() + 0.114*col->blue();
                col->setBlue(bright);
                col->setGreen(bright);
                col->setRed(bright);
                newImage.setPixelColor(i, j, *col);
            }
        delete col;
    }
    return new QPixmap (QPixmap::fromImage(newImage));
}

QPixmap* Graphic::Brightness(int val)       //яркость
{
    QImage newImage;
    if (reserve != nullptr)
    {
        newImage = *new QImage (reserve->pixmap().toImage());
        QColor* col = new QColor ();
        for (int i = 0; i < newImage.width(); i++)
            for (int j = 0; j < newImage.height(); j++)
            {
                delete col;
                col = new QColor (newImage.pixelColor(i, j));
                if (val > 0)
                {
                    col->setBlue(std::min ((col->blue() + val), 255));
                    col->setGreen(std::min ((col->green() + val), 255));
                    col->setRed(std::min ((col->red() + val), 255));
                }
                else
                {
                    col->setBlue(std::max ((col->blue() + val), 0));
                    col->setGreen(std::max ((col->green() + val), 0));
                    col->setRed(std::max ((col->red() + val), 0));
                }
                newImage.setPixelColor(i, j, *col);
            }
        delete col;
    }
    return new QPixmap (QPixmap::fromImage(newImage));
}

QPixmap* Graphic::Contrast (float val)      //контраст
{
    QImage* newImage;
    if (reserve != nullptr)
    {
        newImage = new QImage (reserve->pixmap().toImage());
        QColor* col;
        double r = 0, g = 0, b = 0;
        for (int i = 0; i < newImage->width(); i++)
            for (int j = 0; j < newImage->height(); j++)
            {
                col = new QColor (newImage->pixelColor(i, j));
                r = r + col->red();
                g = g + col->green();
                b = b + col->blue();
                delete col;
            }
        r = r / (newImage->width() * newImage->height());     //средние значения цветов
        g = g / (newImage->width() * newImage->height());
        b = b / (newImage->width() * newImage->height());

        int rn, gn, bn;
        for (int i = 0; i < newImage->width(); i++)
            for (int j = 0; j < newImage->height(); j++)
            {
                col = new QColor (newImage->pixelColor(i, j));
                rn = val*(col->red() - r) + r;
                gn = val*(col->green() - g) + g;
                bn = val*(col->blue() - b) + b;
                rn = std::min (rn, 255);
                gn = std::min (gn, 255);
                bn = std::min (bn, 255);
                rn = std::max (rn, 0);
                gn = std::max (gn, 0);
                bn = std::max (bn, 0);
                col->setRed(rn);
                col->setGreen(gn);
                col->setBlue(bn);
                newImage->setPixelColor(i, j, *col);
                delete col;
            }
        //delete col;
    }
    QPixmap *retpix = new QPixmap (QPixmap::fromImage(*newImage));
    delete newImage;
    return retpix;
}

QPixmap* Graphic::Binarization(int val)       //яркость
{
    QImage newImage;
    if (reserve != nullptr)
    {
        newImage = *new QImage (reserve->pixmap().toImage());
        QColor* col;
        qreal bright;           //яркость
        for (int i = 0; i < newImage.width(); i++)
            for (int j = 0; j < newImage.height(); j++)
            {
                col = new QColor (newImage.pixelColor(i, j));
                bright = 0.299*col->red() + 0.5876*col->green() + 0.114*col->blue();

                if (bright > val)       //ярость больше порога?
                {
                    col->setBlue(255);
                    col->setGreen(255);
                    col->setRed(255);
                }
                else
                {
                    col->setBlue(0);
                    col->setGreen(0);
                    col->setRed(0);
                }
                newImage.setPixelColor(i, j, *col);
                delete col;
            }
        //delete col;
    }
    return new QPixmap (QPixmap::fromImage(newImage));
}


QPixmap* Graphic::Median ()
{
    QImage newImage;
    QPixmap *pxm = new QPixmap (reserve->pixmap().width() + 2, reserve->pixmap().height() + 2);
    QPainter painter (pxm);
    if (reserve != nullptr)
    {
       // newImage = *new QImage (reserve->pixmap().width() + 2, reserve->pixmap().height() + 2, QImage::Format_RGB32); //изображение, большее на пиксель //(reserve->pixmap().toImage(),);
        // (QPixmap::fromImage(newImage));

        painter.drawPixmap(0, 1, 1, reserve->pixmap().height(), reserve->pixmap(), 0, 0, 1, reserve->pixmap().height()); //рисуем столбец
        painter.drawPixmap(reserve->pixmap().width() + 1, 1, 1, reserve->pixmap().height(), reserve->pixmap(), reserve->pixmap().width() - 1, 0, 1, reserve->pixmap().height()); //рисуем столбец

        painter.drawPixmap(1, 0, reserve->pixmap().width(), 1, reserve->pixmap(), 0, 0, reserve->pixmap().width(), 1); //рисуем столбец
        painter.drawPixmap(1, reserve->pixmap().height() + 1, reserve->pixmap().width(), 1, reserve->pixmap(), 0, reserve->pixmap().height() - 1, reserve->pixmap().width(),  1); //рисуем столбец
        painter.drawPixmap(1, 1, reserve->pixmap()); //рисуем на увеличенном пиксмапе

        //int r, g, b;
        int sum;
        QList <int> lst;
        QColor* col = new QColor();
        newImage = *new QImage (pxm->toImage());
        for (int i = 1; i < pxm->width() - 1; i++)
            for (int j = 1; j < pxm->height() - 1; j++)
            {
                col = new QColor (newImage.pixelColor(i, j));
//                sum = 0;
                lst.append(newImage.pixelColor(i, j-1).red());
                lst.append(newImage.pixelColor(i, j).red());
                lst.append(newImage.pixelColor(i, j+1).red());
                lst.append(newImage.pixelColor(i-1, j-1).red());
                lst.append(newImage.pixelColor(i-1, j).red());
                lst.append(newImage.pixelColor(i-1, j+1).red());
                lst.append(newImage.pixelColor(i+1, j-1).red());
                lst.append(newImage.pixelColor(i+1, j).red());
                lst.append(newImage.pixelColor(i+1, j+1).red());
//                sum += newImage.pixelColor(i-1, j-1).red();
//                sum += newImage.pixelColor(i-1, j).red();
//                sum += newImage.pixelColor(i-1, j+1).red();
//                sum += newImage.pixelColor(i, j-1).red();
//                sum += newImage.pixelColor(i, j).red();
//                sum += newImage.pixelColor(i, j+1).red();
//                sum += newImage.pixelColor(i+1, j-1).red();
//                sum += newImage.pixelColor(i+1, j).red();
//                sum += newImage.pixelColor(i+1, j+1).red();

                qSort (lst.begin(), lst.end());
                col->setRed(lst[4]);
                lst.clear();
                lst.append(newImage.pixelColor(i, j-1).blue());
                lst.append(newImage.pixelColor(i, j).blue());
                lst.append(newImage.pixelColor(i, j+1).blue());
                lst.append(newImage.pixelColor(i-1, j-1).blue());
                lst.append(newImage.pixelColor(i-1, j).blue());
                lst.append(newImage.pixelColor(i-1, j+1).blue());
                lst.append(newImage.pixelColor(i+1, j-1).blue());
                lst.append(newImage.pixelColor(i+1, j).blue());
                lst.append(newImage.pixelColor(i+1, j+1).blue());
//                sum = 0;
//                sum += newImage.pixelColor(i-1, j-1).blue();
//                sum += newImage.pixelColor(i-1, j).blue();
//                sum += newImage.pixelColor(i-1, j+1).blue();
//                sum += newImage.pixelColor(i, j-1).blue();
//                sum += newImage.pixelColor(i, j).blue();
//                sum += newImage.pixelColor(i, j+1).blue();
//                sum += newImage.pixelColor(i+1, j-1).blue();
//                sum += newImage.pixelColor(i+1, j).blue();
//                sum += newImage.pixelColor(i+1, j+1).blue();
                qSort (lst.begin(), lst.end());
                col->setBlue(lst[4]);
                lst.clear();
                lst.append(newImage.pixelColor(i, j-1).green());
                lst.append(newImage.pixelColor(i, j).green());
                lst.append(newImage.pixelColor(i, j+1).green());
                lst.append(newImage.pixelColor(i-1, j-1).green());
                lst.append(newImage.pixelColor(i-1, j).green());
                lst.append(newImage.pixelColor(i-1, j+1).green());
                lst.append(newImage.pixelColor(i+1, j-1).green());
                lst.append(newImage.pixelColor(i+1, j).green());
                lst.append(newImage.pixelColor(i+1, j+1).green());
//                sum = 0;
//                sum += newImage.pixelColor(i-1, j-1).green();
//                sum += newImage.pixelColor(i-1, j).green();
//                sum += newImage.pixelColor(i-1, j+1).green();
//                sum += newImage.pixelColor(i, j-1).green();
//                sum += newImage.pixelColor(i, j).green();
//                sum += newImage.pixelColor(i, j+1).green();
//                sum += newImage.pixelColor(i+1, j-1).green();
//                sum += newImage.pixelColor(i+1, j).green();
//                sum += newImage.pixelColor(i+1, j+1).green();
                qSort (lst.begin(), lst.end());
                col->setGreen(lst[4]);
                lst.clear();

                painter.setPen(*col);
                painter.drawPoint(i,j);

            }
        delete col;
    }

    return new QPixmap (pxm->copy(1, 1, reserve->pixmap().width(), reserve->pixmap().height()));//new QPixmap (QPixmap::fromImage(newImage));
}

QPixmap* Graphic::Gauss ()
{
    QImage newImage;
    QPixmap *pxm = new QPixmap (reserve->pixmap().width() + 2, reserve->pixmap().height() + 2);
    QPainter painter (pxm);
    if (reserve != nullptr)
    {
        qreal mask[9];
        qreal A;
        qreal ptrSum = 0;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                ptrSum = ptrSum + qPow(M_E, -((i-1)*(i-1)+(j-1)*(j-1))/(SIGMA*SIGMA));
            }
        A = 1/ptrSum;
        for (int i = 0; i < 3; i++)         //находим коэффициенты матрицы
            for (int j = 0; j < 3; j++)
            {
                mask[i*3 + j] = A * qPow(M_E, -((i-1)*(i-1)+(j-1)*(j-1))/(SIGMA*SIGMA));//ptrSum = ptrSum + qPow(M_E, -((i-1)*(i-1)+(j-1)*(j-1))/(SIGMA*SIGMA));
            }

        painter.drawPixmap(0, 1, 1, reserve->pixmap().height(), reserve->pixmap(), 0, 0, 1, reserve->pixmap().height()); //рисуем столбец
        painter.drawPixmap(reserve->pixmap().width() + 1, 1, 1, reserve->pixmap().height(), reserve->pixmap(), reserve->pixmap().width() - 1, 0, 1, reserve->pixmap().height()); //рисуем столбец

        painter.drawPixmap(1, 0, reserve->pixmap().width(), 1, reserve->pixmap(), 0, 0, reserve->pixmap().width(), 1); //рисуем столбец
        painter.drawPixmap(1, reserve->pixmap().height() + 1, reserve->pixmap().width(), 1, reserve->pixmap(), 0, reserve->pixmap().height() - 1, reserve->pixmap().width(),  1); //рисуем столбец
        painter.drawPixmap(1, 1, reserve->pixmap()); //рисуем на увеличенном пиксмапе


        QList <int> lst;
        qreal r, g, b;
        QColor* col = new QColor();
        newImage = *new QImage (pxm->toImage());
        for (int i = 1; i < pxm->width() - 1; i++)
            for (int j = 1; j < pxm->height() - 1; j++)
            {
                r = 0;
                for (int i1 = 0; i1 < 3; i1++)         //находим коэффициенты матрицы
                    for (int j1 = 0; j1 < 3; j1++)
                    {
                        r = r + mask[i1*3 + j1] * newImage.pixelColor(i-1 + i1, j- 1 + j1).red();
                    }
                g = 0;
                for (int i1 = 0; i1 < 3; i1++)         //находим коэффициенты матрицы
                    for (int j1 = 0; j1 < 3; j1++)
                    {
                        g = g + mask[i1*3 + j1] * newImage.pixelColor(i -1 + i1, j-1 + j1).green();
                    }
                b = 0;
                for (int i1 = 0; i1 < 3; i1++)         //находим коэффициенты матрицы
                    for (int j1 = 0; j1 < 3; j1++)
                    {
                        b = b + mask[i1*3 + j1] * newImage.pixelColor(i -1 + i1, j-1 + j1).blue();
                    }

//                col->setRed(r);
//                col->setGreen(g);
//                col->setBlue(b);
                col= new QColor (r, g, b);
                painter.setPen(*col);
                painter.drawPoint(i, j);
                delete col;
            }

    }
    SIGMA = 0.8;        //на всякий случай устанавливаем по умолчанию
    return new QPixmap (pxm->copy(1, 1, reserve->pixmap().width(), reserve->pixmap().height()));//new QPixmap (QPixmap::fromImage(newImage));
}


QPixmap* Graphic::Rezk ()
{
    QImage newImage;
    QPixmap *pxm = new QPixmap (reserve->pixmap().width() + 2, reserve->pixmap().height() + 2);
    QPainter painter (pxm);
    int k = 8;
    if (reserve != nullptr)
    {
        qreal mask[9];

        for (int i = 0; i < 3; i++)         //находим коэффициенты матрицы
            for (int j = 0; j < 3; j++)
            {
                mask[i*3 + j] = -k/8;//ptrSum = ptrSum + qPow(M_E, -((i-1)*(i-1)+(j-1)*(j-1))/(SIGMA*SIGMA));
            }
        mask[4] = k + 1;



        painter.drawPixmap(0, 1, 1, reserve->pixmap().height(), reserve->pixmap(), 0, 0, 1, reserve->pixmap().height()); //рисуем столбец
        painter.drawPixmap(reserve->pixmap().width() + 1, 1, 1, reserve->pixmap().height(), reserve->pixmap(), reserve->pixmap().width() - 1, 0, 1, reserve->pixmap().height()); //рисуем столбец

        painter.drawPixmap(1, 0, reserve->pixmap().width(), 1, reserve->pixmap(), 0, 0, reserve->pixmap().width(), 1); //рисуем столбец
        painter.drawPixmap(1, reserve->pixmap().height() + 1, reserve->pixmap().width(), 1, reserve->pixmap(), 0, reserve->pixmap().height() - 1, reserve->pixmap().width(),  1); //рисуем столбец
        painter.drawPixmap(1, 1, reserve->pixmap()); //рисуем на увеличенном пиксмапе

        qreal r, g, b;
        QColor* col;// = new QColor();
        newImage = *new QImage (pxm->toImage());
        for (int i = 1; i < pxm->width() - 1; i++)
            for (int j = 1; j < pxm->height() - 1; j++)
            {
                r = 0;
                for (int i1 = 0; i1 < 3; i1++)         //находим коэффициенты матрицы
                    for (int j1 = 0; j1 < 3; j1++)
                    {
                        r = r + mask[i1*3 + j1] * newImage.pixelColor(i-1 + i1, j- 1 + j1).red();
                    }
                g = 0;
                for (int i1 = 0; i1 < 3; i1++)         //находим коэффициенты матрицы
                    for (int j1 = 0; j1 < 3; j1++)
                    {
                        g = g + mask[i1*3 + j1] * newImage.pixelColor(i -1 + i1, j-1 + j1).green();
                    }
                b = 0;
                for (int i1 = 0; i1 < 3; i1++)         //находим коэффициенты матрицы
                    for (int j1 = 0; j1 < 3; j1++)
                    {
                        b = b + mask[i1*3 + j1] * newImage.pixelColor(i -1 + i1, j-1 + j1).blue();
                    }
                r = std::max (r, 0.);       //приводим цвета к диапазону
                g = std::max (g, 0.);
                b = std::max (b, 0.);
                r = std::min (r, 255.);
                g = std::min (g, 255.);
                b = std::min (b, 255.);
                col= new QColor ((int)r, (int)g, (int)b);
                painter.setPen(*col);
                painter.drawPoint(i, j);
                delete col;
            }

    }
    return new QPixmap (pxm->copy(1, 1, reserve->pixmap().width(), reserve->pixmap().height()));
}


QPixmap* Graphic::MyFilter (int k)  //мой фильтр
{
    QImage newImage;
    QPixmap *pxm = new QPixmap (reserve->pixmap().width() + 2, reserve->pixmap().height() + 2);
    QPainter painter (pxm);
    //int k = 8;
    if (reserve != nullptr)
    {
        qreal mask[9];

        for (int i = 0; i < 3; i++)         //находим коэффициенты матрицы
            for (int j = 0; j < 3; j++)
            {
                mask[i*3 + j] = -k/8;//ptrSum = ptrSum + qPow(M_E, -((i-1)*(i-1)+(j-1)*(j-1))/(SIGMA*SIGMA));
            }
        mask[4] = k + 1;



        painter.drawPixmap(0, 1, 1, reserve->pixmap().height(), reserve->pixmap(), 0, 0, 1, reserve->pixmap().height()); //рисуем столбец
        painter.drawPixmap(reserve->pixmap().width() + 1, 1, 1, reserve->pixmap().height(), reserve->pixmap(), reserve->pixmap().width() - 1, 0, 1, reserve->pixmap().height()); //рисуем столбец

        painter.drawPixmap(1, 0, reserve->pixmap().width(), 1, reserve->pixmap(), 0, 0, reserve->pixmap().width(), 1); //рисуем столбец
        painter.drawPixmap(1, reserve->pixmap().height() + 1, reserve->pixmap().width(), 1, reserve->pixmap(), 0, reserve->pixmap().height() - 1, reserve->pixmap().width(),  1); //рисуем столбец
        painter.drawPixmap(1, 1, reserve->pixmap()); //рисуем на увеличенном пиксмапе

        qreal r, g, b;
        QColor* col;// = new QColor();
        newImage = *new QImage (pxm->toImage());
        for (int i = 1; i < pxm->width() - 1; i++)
            for (int j = 1; j < pxm->height() - 1; j++)
            {
                r = 0;
                for (int i1 = 0; i1 < 3; i1++)         //находим коэффициенты матрицы
                    for (int j1 = 0; j1 < 3; j1++)
                    {
                        r = r + mask[i1*3 + j1] * newImage.pixelColor(i-1 + i1, j- 1 + j1).red();
                    }
                g = 0;
                for (int i1 = 0; i1 < 3; i1++)         //находим коэффициенты матрицы
                    for (int j1 = 0; j1 < 3; j1++)
                    {
                        g = g + mask[i1*3 + j1] * newImage.pixelColor(i -1 + i1, j-1 + j1).green();
                    }
                b = 0;
                for (int i1 = 0; i1 < 3; i1++)         //находим коэффициенты матрицы
                    for (int j1 = 0; j1 < 3; j1++)
                    {
                        b = b + mask[i1*3 + j1] * newImage.pixelColor(i -1 + i1, j-1 + j1).blue();
                    }
//                r = std::max (r, 0.);
//                g = std::max (g, 0.);
//                b = std::max (b, 0.);
                if (r >= 255 || r <= 0) {r = 0; g = 0; b = 0;}
                if (g >= 255 || g <= 0) {r = 0; g = 0; b = 0;}
                if (b >= 255 || b <= 0) {r = 0; g = 0; b = 0;}
                col= new QColor (r, g, b);
                painter.setPen(*col);
                painter.drawPoint(i, j);
                delete col;
            }

    }
    return new QPixmap (pxm->copy(1, 1, reserve->pixmap().width(), reserve->pixmap().height()));
}

bool Graphic::setYUVMatix()
{
    int width = image->width();
    int height = image->height();

    if(image->format()!=QImage::Format_RGB32    && image->format() != QImage::Format_ARGB32)
    {
       printf("Wrong image format\n");
       return false;
    }

    // RGB32 to YUV420
    int size = width * height;
    // Y
    Y = new unsigned char [size];
    U = new unsigned char [size];
    V = new unsigned char [size];

    QColor tempColor;
    for (int i = 0; i < width; i++)
       for (int j = 0; j < height; j++)
       {
         tempColor = image->pixelColor(i, j);//Canvas->Pixels[i][j];
         int r = tempColor.red();
         int g = tempColor.green();
         int b = tempColor.blue();


         Y[j * width + i] = (0.299 * r + 0.587 * g + 0.114 * b);
         U[j * width + i] = (-0.14713 * r - 0.28886 * g + 0.436 * b + 128);
         V[j * width + i] = (0.615 * r - 0.51499 * g - 0.10001 * b + 128);
       }
    return true;
}

bool Graphic::setYUV()
{
    if (image != nullptr)
    {
//        delete
//        image = new QImage (*image);
        //newImage.invertPixels();
        QColor* col = new QColor ();
        int w = image->width();
        int h = image->height();
        for (int i = 0; i < w; i++)
            for (int j = 0; j < h; j++)
            {
                delete col;
                col = new QColor (image->pixelColor(i, j));
                col->setRed(Y[j * w + i]);
                col->setGreen(U[j * w + i]);
                col->setBlue(V[j * w + i]);

                image->setPixelColor(i, j, *col);
            }
        delete col;
    }
    return true;//return new QPixmap (QPixmap::fromImage(newImage));
}

QColor Graphic::matrixColorMul(QColor colors[3][3], int matrix[3][3])
{
    int r = 0, g = 0, b = 0;
    r = colors[0][0].red() * matrix[0][0] + colors[0][1].red() * matrix[0][1] + colors[0][2].red() * matrix[0][2] +
            colors[1][0].red() * matrix[1][0] + colors[1][1].red() * matrix[1][1] + colors[1][2].red() * matrix[1][2] +
            colors[2][0].red() * matrix[2][0] + colors[2][1].red() * matrix[2][1] + colors[2][2].red() * matrix[2][2];
    g = colors[0][0].green() * matrix[0][0] + colors[0][1].green() * matrix[0][1] + colors[0][2].green() * matrix[0][2] +
            colors[1][0].green() * matrix[1][0] + colors[1][1].green() * matrix[1][1] + colors[1][2].green() * matrix[1][2] +
            colors[2][0].green() * matrix[2][0] + colors[2][1].green() * matrix[2][1] + colors[2][2].green() * matrix[2][2];
    b = colors[0][0].blue() * matrix[0][0] + colors[0][1].blue() * matrix[0][1] + colors[0][2].blue() * matrix[0][2] +
            colors[1][0].blue() * matrix[1][0] + colors[1][1].blue() * matrix[1][1] + colors[1][2].blue() * matrix[1][2] +
            colors[2][0].blue() * matrix[2][0] + colors[2][1].blue() * matrix[2][1] + colors[2][2].blue() * matrix[2][2];
    r = qMax (0,
              qMin (r, 255));
    g = qMax (0,
              qMin (g, 255));
    b = qMax (0,
              qMin (b, 255));
    return QColor (r, g, b);
}

unsigned char Graphic::matrixMul(unsigned char channel[3][3], int matrix[3][3])
{
    int c = 0;
    c = channel[0][0] * matrix[0][0] + channel[0][1] * matrix[0][1] + channel[0][2] * matrix[0][2] +
            channel[1][0] * matrix[1][0] + channel[1][1] * matrix[1][1] + channel[1][2] * matrix[1][2] +
            channel[2][0] * matrix[2][0] + channel[2][1] * matrix[2][1] + channel[2][2] * matrix[2][2];
    c = qMax (0,
              qMin (c, 255));
    return c;
}

QColor Graphic::colorNormir(QColor colorX, QColor colorY)
{
    double rx = colorX.red(), gx = colorX.green(), bx = colorX.blue();
    double ry = colorY.red(), gy = colorY.green(), by = colorY.blue();
    rx = sqrt(rx * rx + ry * ry);
    gx = sqrt(gx * gx + gy * gy);
    bx = sqrt(bx * bx + by * by);
    int r = qMax (0,
              qMin (static_cast <int> (rx), 255));
    int g = qMax (0,
              qMin (static_cast <int>(gx), 255));
    int b = qMax (0,
              qMin (static_cast <int>(bx), 255));
    return QColor (r, g, b);
}

unsigned char Graphic::componentNormir(unsigned char colorX, unsigned char colorY)
{
    return static_cast <unsigned char> (qMax (0,
                 qMin (static_cast <int> (sqrt(colorX * colorX + colorY * colorY)), 255)));
}

QImage* Graphic::sobelOperator()
{
    int sobelMaskY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int sobelMaskX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};


    QColor col[3][3];

    QImage newImage = QImage (*image);
    int w = image->width();
    int h = image->height();
//    float r,g,b;
    int kx, ky;     //обозначают границы
    for (int i = 0; i < w; i++) {
        kx = -1;
        ky = -1;
        for (int j = 0; j < h; j++) {
            col[0][0] = newImage.pixelColor(i - (kx < 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
            col[0][1] = newImage.pixelColor(i - (kx < 0 ? 0 : 1), j);
            col[0][2] = newImage.pixelColor(i - (kx < 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
            col[1][0] = newImage.pixelColor(i, j - (ky < 0 ? 0 : 1));
            col[1][1] = newImage.pixelColor(i, j);
            col[1][2] = newImage.pixelColor(i, j + (ky > 0 ? 0 : 1));
            col[2][0] = newImage.pixelColor(i + (kx > 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
            col[2][1] = newImage.pixelColor(i + (kx > 0 ? 0 : 1), j);
            col[2][2] = newImage.pixelColor(i + (kx > 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
            QColor color = colorNormir(matrixColorMul(col, sobelMaskX), matrixColorMul(col, sobelMaskY));

            image->setPixelColor (i, j, color);

            if (i == w - 1)
                ky = 1;
            else
                ky = 0;
            if (j == h - 1)
                kx = 1;
            else //if (j != 0)
                    kx = 0;
//                else
//                    ky = -1;
        }
    }

    return image;
}

QImage* Graphic::sobelOperatorOneChannel(unsigned char *matrix)
{
    int sobelMaskY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int sobelMaskX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int w = image->width();
    int h = image->height();
    unsigned char col[3][3];

//    QImage newImage = QImage (*image);
    int kx, ky;     //обозначают границы

    for (int i = 0; i < w; i++) {
        kx = -1;
        ky = -1;
        for (int j = 0; j < h; j++) {
            col[0][0] = matrix[(j - (ky < 0 ? 0 : 1)) * w + i - (kx < 0 ? 0 : 1)];
            col[0][1] = matrix[(j - (ky < 0 ? 0 : 1)) * w + i];
            col[0][2] = matrix[(j - (ky < 0 ? 0 : 1)) * w +  i + (kx > 0 ? 0 : 1)];
            col[1][0] = matrix[j * w +  i - (kx < 0 ? 0 : 1)];
            col[1][1] = matrix[j * w +  i];
            col[1][2] = matrix[(j) * w +  i + (kx > 0 ? 0 : 1)];
            col[2][0] = matrix[(j + (ky > 0 ? 0 : 1)) * w +  i - (kx < 0 ? 0 : 1)];
            col[2][1] = matrix[(j + (ky > 0 ? 0 : 1)) * w +  i];
            col[2][2] = matrix[(j + (ky > 0 ? 0 : 1)) * w +  i + (kx > 0 ? 0 : 1)];

            unsigned char channelVal = (componentNormir(matrixMul(col, sobelMaskX), matrixMul(col, sobelMaskY)) >= 255 - LIMIT) ? 255 : 0;

            image->setPixelColor (i, j, QColor(channelVal, channelVal, channelVal));
            if (i == w - 1)
                kx = 1;
            else
                kx = 0;
            if (j == h - 1)
                ky = 1;
            else //if (j != 0)
                    ky = 0;
//                else
//                    ky = -1;
        }

    }

    return image;
}


QImage *Graphic::getImage() const
{
    return image;
}

void Graphic::setImage(QImage *value)
{
    image = new QImage (*value);
}

unsigned char *Graphic::getY() const
{
    return Y;
}

unsigned char *Graphic::getU() const
{
    return U;
}

unsigned char *Graphic::getV() const
{
    return V;
}

void Graphic::setLIMIT(int value)
{
    LIMIT = value;
}

void Graphic::resizeEvent(QResizeEvent *event)
{
    //QGraphicsView::resizeEvent(event);
    //emit ResizeSignal();

}

