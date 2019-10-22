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

QColor Graphic::matrixMul(QColor colors[3][3], int matrix[3][3])
{
    int r = 0, g = 0, b = 0;
    r = colors[0][0].red() * matrix[0][0] + colors[0][1].red() * matrix[0][1] + colors[0][2].red() * matrix[0][2] +
            colors[1][0].red() * matrix[1][0] + colors[1][1].red() * matrix[1][1] + colors[1][2].red() * matrix[1][2] +
            colors[2][0].red() * matrix[2][0] + colors[2][1].red() * matrix[2][1] + colors[2][2].red() * matrix[2][2];
    g = colors[0][0].green() * matrix[0][0] + colors[0][1].green() * matrix[0][1] + colors[0][2].green() * matrix[0][2] +
            colors[1][0].green() * matrix[1][0] + colors[1][1].green() * matrix[1][1] + colors[1][2].green() * matrix[1][2] +
            colors[2][0].green() * matrix[2][0] + colors[2][1].green() * matrix[2][1] + colors[2][2].green() * matrix[2][2];
    g = colors[0][0].blue() * matrix[0][0] + colors[0][1].blue() * matrix[0][1] + colors[0][2].blue() * matrix[0][2] +
            colors[1][0].blue() * matrix[1][0] + colors[1][1].blue() * matrix[1][1] + colors[1][2].blue() * matrix[1][2] +
            colors[2][0].blue() * matrix[2][0] + colors[2][1].blue() * matrix[2][1] + colors[2][2].blue() * matrix[2][2];
    return QColor (r, g, b);
}

QColor Graphic::colorNormir(QColor colorX, QColor colorY)
{
    qreal rx = colorX.redF(), gx = colorX.greenF(), bx = colorX.blueF();
    qreal ry = colorY.redF(), gy = colorY.greenF(), by = colorY.blueF();
    rx = sqrt(rx * rx + ry * ry);
    gx = sqrt(gx * gx + gy * gy);
    bx = sqrt(bx * bx + by * by);
    return QColor (rx, gx, bx);
}

bool Graphic::sobelOperator(QImage image)
{
    int sobelMaskY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int sobelMaskX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int w = image.width();
    int h = image.height();
    QColor col[3][3];

    QImage newImage = QImage (image);
    float r,g,b;
    int kx, ky = -1;     //обозначают границы
    for (int i = 0; i < w; i++) {
        kx = -1;
        for (int j = 0; j < h; j++) {
            col[0][0] = image.pixelColor(i - (kx < 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
            col[0][1] = image.pixelColor(i - (kx < 0 ? 0 : 1), j);
            col[0][2] = image.pixelColor(i - (kx < 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
            col[1][0] = image.pixelColor(i, j - (ky < 0 ? 0 : 1));
            col[1][1] = image.pixelColor(i, j);
            col[1][2] = image.pixelColor(i, j + (ky > 0 ? 0 : 1));
            col[2][0] = image.pixelColor(i + (kx > 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
            col[2][1] = image.pixelColor(i + (kx > 0 ? 0 : 1), j);
            col[2][2] = image.pixelColor(i + (kx > 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
            r = col[1][1].red();
            g = col[1][1].green();
            b = col[1][1].blue();

        }
    }
//    image

//    QImage newImage;
//    if (reserve != nullptr)
//    {
//        newImage = *new QImage (reserve->pixmap().toImage());
//        QColor* col;
//        qreal bright;           //яркость
//        for (int i = 0; i < newImage.width(); i++)
//            for (int j = 0; j < newImage.height(); j++)
//            {
//                col = new QColor (newImage.pixelColor(i, j));
//                bright = 0.299*col->red() + 0.5876*col->green() + 0.114*col->blue();

//                if (bright > val)       //ярость больше порога?
//                {
//                    col->setBlue(255);
//                    col->setGreen(255);
//                    col->setRed(255);
//                }
//                else
//                {
//                    col->setBlue(0);
//                    col->setGreen(0);
//                    col->setRed(0);
//                }
//                newImage.setPixelColor(i, j, *col);
//                delete col;
//            }
//        //delete col;
//    }
//    return new QPixmap (QPixmap::fromImage(newImage));
}

QImage *Graphic::getImage() const
{
    return image;
}

void Graphic::setImage(QImage *value)
{
    image = value;
}

void Graphic::resizeEvent(QResizeEvent *event)
{
    //QGraphicsView::resizeEvent(event);
    //emit ResizeSignal();

}

