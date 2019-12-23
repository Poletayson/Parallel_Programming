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


void Graphic::Binarization()       //яркость
{
    int w = image->width();
    int h = image->height();

    QRgb *imageBytes[h];
    for (int i = 0; i < h; i++){
        imageBytes[i] = (QRgb*)(image->scanLine(i));
    }

    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            int bright = static_cast<int> (0.299 * qRed(imageBytes[j][i]) + 0.5876 * qGreen(imageBytes[j][i]) + 0.114 * qBlue(imageBytes[j][i]));    //яркость

            if (bright > LIMIT)       //ярость больше порога?
            {
                imageBytes[j][i] = qRgb(255, 255, 255);
            }
            else
            {
                imageBytes[j][i] = qRgb(0, 0, 0);
            }
        }

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
//        QColor* col = new QColor ();
        int w = image->width();
        int h = image->height();

        QRgb *imageBytes[h];
        for (int i = 0; i < h; i++){
            imageBytes[i] = (QRgb*)(image->scanLine(i));
        }

        for (int i = 0; i < w; i++)
            for (int j = 0; j < h; j++)
            {
                imageBytes[j][i] = qRgb(Y[j * w + i], U[j * w + i], V[j * w + i]);
            }



//        for (int i = 0; i < w; i++)
//            for (int j = 0; j < h; j++)
//            {
//                delete col;
//                col = new QColor (image->pixelColor(i, j));
//                col->setRed(Y[j * w + i]);
//                col->setGreen(U[j * w + i]);
//                col->setBlue(V[j * w + i]);

//                image->setPixelColor(i, j, *col);
//            }
//        delete col;
        return true;
    }
    else return false;
}

bool Graphic::setYUVMatixOMP()
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

#pragma omp parallel for
    for (int i = 0; i < width; i++)
       for (int j = 0; j < height; j++)
       {
         QColor tempColor = image->pixelColor(i, j);//Canvas->Pixels[i][j];
         int r = tempColor.red();
         int g = tempColor.green();
         int b = tempColor.blue();

         Y[j * width + i] = (0.299 * r + 0.587 * g + 0.114 * b);
         U[j * width + i] = (-0.14713 * r - 0.28886 * g + 0.436 * b + 128);
         V[j * width + i] = (0.615 * r - 0.51499 * g - 0.10001 * b + 128);
       }
    return true;
}

bool Graphic::setYUVOMP()
{
    if (image != nullptr)
    {

        int w = image->width();
        int h = image->height();

        QRgb *imageBytes[h];
        for (int i = 0; i < h; i++){
            imageBytes[i] = (QRgb*)(image->scanLine(i));
        }

        #pragma omp parallel for
        for (int i = 0; i < w; i++)
            for (int j = 0; j < h; j++)
            {
                imageBytes[j][i] = qRgb(Y[j * w + i], U[j * w + i], V[j * w + i]);
            }
        return true;
    }
    else return false;
}

void Graphic::sobelOperatorOMP()
{
    int sobelMaskY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int sobelMaskX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int w = image->width();
    int h = image->height();

    QRgb *imageBytes[h];
    QRgb *newImageBytes[h];
    for (int i = 0; i < h; i++){
        imageBytes[i] = (QRgb*)(image->scanLine(i));
    }

#pragma omp parallel for
    for (int i = 0; i < h; i++){
        newImageBytes[i] = new QRgb[w];
        for (int j = 0; j < w; j++){
            newImageBytes[i][j] = imageBytes[i][j];
        }
    }

#pragma omp parallel for
    for (int i = 1; i < w-1; i++) {
        int kx = -1;        //обозначают границы
        int ky = -1;
        for (int j = 1; j < h-1; j++) {
            QRgb col[3][3];
            col[0][0] = newImageBytes[j - (ky < 0 ? 0 : 1)][i - (kx < 0 ? 0 : 1)];//newImage.pixelColor(i - (kx < 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
            col[0][1] = newImageBytes[j][i - (kx < 0 ? 0 : 1)];//newImage.pixelColor(i - (kx < 0 ? 0 : 1), j);
            col[0][2] = newImageBytes[j + (ky > 0 ? 0 : 1)][i - (kx < 0 ? 0 : 1)];//newImage.pixelColor(i - (kx < 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
            col[1][0] = newImageBytes[j][i];//newImage.pixelColor(i, j - (ky < 0 ? 0 : 1));
            col[1][1] = newImageBytes[j + (ky > 0 ? 0 : 1)][i];//newImage.pixelColor(i, j);
            col[1][2] = newImageBytes[j - (ky < 0 ? 0 : 1)][i];//newImage.pixelColor(i, j + (ky > 0 ? 0 : 1));
            col[2][0] = newImageBytes[j - (ky < 0 ? 0 : 1)][i + (kx > 0 ? 0 : 1)];//newImage.pixelColor(i + (kx > 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
            col[2][1] = newImageBytes[j][i + (kx > 0 ? 0 : 1)];//newImage.pixelColor(i + (kx > 0 ? 0 : 1), j);
            col[2][2] = newImageBytes[j + (ky > 0 ? 0 : 1)][i + (kx > 0 ? 0 : 1)];//newImage.pixelColor(i + (kx > 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
            QRgb color = colorNormir(matrixColorMul(col, sobelMaskX), matrixColorMul(col, sobelMaskY));


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
            imageBytes[j][i] = color;

        }
    }
}

void Graphic::BinarizationOMP()
{
    int w = image->width();
    int h = image->height();

    QRgb *imageBytes[h];
    for (int i = 0; i < h; i++){
        imageBytes[i] = (QRgb*)(image->scanLine(i));
    }

    #pragma omp parallel for
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            int bright = static_cast<int> (0.299 * qRed(imageBytes[j][i]) + 0.5876 * qGreen(imageBytes[j][i]) + 0.114 * qBlue(imageBytes[j][i]));    //яркость

            if (bright > LIMIT)       //ярость больше порога?
            {
                imageBytes[j][i] = qRgb(255, 255, 255);
            }
            else
            {
                imageBytes[j][i] = qRgb(0, 0, 0);
            }
        }
}

bool Graphic::setYUVMatixMPI()
{
    if(image->format()!=QImage::Format_RGB32    && image->format() != QImage::Format_ARGB32)
    {
       printf("Wrong image format\n");
       return false;
    }
    int procRank, procSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);   //получаем номер процесса
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);


    int x1 = width/procSize * procRank; //начало
    //if (x1 == 0) ++x1;
    int lineH = height;
    int x2 = (procRank != procSize - 1) ? x1 + width/procSize : width;

    for (int i = x1; i < x2; i++)
       for (int j = 0; j < height; j++)
       {
         QColor tempColor = image->pixelColor(i, j);//Canvas->Pixels[i][j];
         int r = tempColor.red();
         int g = tempColor.green();
         int b = tempColor.blue();

         Y[j * width + i] = (0.299 * r + 0.587 * g + 0.114 * b);
         U[j * width + i] = (-0.14713 * r - 0.28886 * g + 0.436 * b + 128);
         V[j * width + i] = (0.615 * r - 0.51499 * g - 0.10001 * b + 128);
       }
    return true;
}

bool Graphic::setYUVMPI()
{
    if (image != nullptr)
    {
        int procRank, procSize;
        MPI_Comm_rank(MPI_COMM_WORLD, &procRank);   //получаем номер процесса
        MPI_Comm_size(MPI_COMM_WORLD, &procSize);


        int x1 = width/procSize * procRank; //начало
        //if (x1 == 0) ++x1;
        int lineH = height;
        int x2 = (procRank != procSize - 1) ? x1 + width/procSize : width;

        int w = image->width();
        int h = image->height();

        QRgb *imageBytes[h];
        for (int i = 0; i < h; i++){
            imageBytes[i] = (QRgb*)(image->scanLine(i));
        }

        for (int i = x1; i < x2; i++)
            for (int j = 0; j < h; j++)
            {
                imageBytes[j][i] = qRgb(Y[j * w + i], U[j * w + i], V[j * w + i]);
            }
        return true;
    }
    else return false;
}

void Graphic::sobelOperatorMPI()
{
    int sobelMaskY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int sobelMaskX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int w = image->width();
    int h = image->height();

    int procRank, procSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);   //получаем номер процесса
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);


    int x1 = width/procSize * procRank; //начало
    if (x1 == 0) ++x1;
    int lineH = height;
    int x2 = (procRank != procSize - 1) ? x1 + width/procSize : width - 1;

    QRgb *imageBytes[h];
    QRgb *newImageBytes[h];
    for (int i = 0; i < h; i++){
        imageBytes[i] = (QRgb*)(image->scanLine(i));
    }

//#pragma omp parallel for
    for (int i = 0; i < h; i++){
        newImageBytes[i] = new QRgb[w];
        for (int j = 0; j < w; j++){
            newImageBytes[i][j] = imageBytes[i][j];
        }
    }

//#pragma omp parallel for
    for (int i = x1; i < x2; i++) {
        int kx = -1;        //обозначают границы
        int ky = -1;
        for (int j = 1; j < h-1; j++) {
            QRgb col[3][3];
            col[0][0] = newImageBytes[j - (ky < 0 ? 0 : 1)][i - (kx < 0 ? 0 : 1)];//newImage.pixelColor(i - (kx < 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
            col[0][1] = newImageBytes[j][i - (kx < 0 ? 0 : 1)];//newImage.pixelColor(i - (kx < 0 ? 0 : 1), j);
            col[0][2] = newImageBytes[j + (ky > 0 ? 0 : 1)][i - (kx < 0 ? 0 : 1)];//newImage.pixelColor(i - (kx < 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
            col[1][0] = newImageBytes[j][i];//newImage.pixelColor(i, j - (ky < 0 ? 0 : 1));
            col[1][1] = newImageBytes[j + (ky > 0 ? 0 : 1)][i];//newImage.pixelColor(i, j);
            col[1][2] = newImageBytes[j - (ky < 0 ? 0 : 1)][i];//newImage.pixelColor(i, j + (ky > 0 ? 0 : 1));
            col[2][0] = newImageBytes[j - (ky < 0 ? 0 : 1)][i + (kx > 0 ? 0 : 1)];//newImage.pixelColor(i + (kx > 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
            col[2][1] = newImageBytes[j][i + (kx > 0 ? 0 : 1)];//newImage.pixelColor(i + (kx > 0 ? 0 : 1), j);
            col[2][2] = newImageBytes[j + (ky > 0 ? 0 : 1)][i + (kx > 0 ? 0 : 1)];//newImage.pixelColor(i + (kx > 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
            QRgb color = colorNormir(matrixColorMul(col, sobelMaskX), matrixColorMul(col, sobelMaskY));


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
            imageBytes[j][i] = color;

        }
    }
}

void Graphic::BinarizationMPI()
{
    int w = image->width();
    int h = image->height();

    QRgb *imageBytes[h];
    for (int i = 0; i < h; i++){
        imageBytes[i] = (QRgb*)(image->scanLine(i));
    }

    int procRank, procSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);   //получаем номер процесса
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);


    int x1 = width/procSize * procRank; //начало
    //if (x1 == 0) ++x1;
    int lineH = height;
    int x2 = (procRank != procSize - 1) ? x1 + width/procSize : width;

    for (int i = x1; i < x2; i++)
        for (int j = 0; j < h; j++)
        {
            int bright = static_cast<int> (0.299 * qRed(imageBytes[j][i]) + 0.5876 * qGreen(imageBytes[j][i]) + 0.114 * qBlue(imageBytes[j][i]));    //яркость

            if (bright > LIMIT)       //ярость больше порога?
            {
                imageBytes[j][i] = qRgb(255, 255, 255);
            }
            else
            {
                imageBytes[j][i] = qRgb(0, 0, 0);
            }
        }
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

QRgb Graphic::matrixColorMul(QRgb colors[3][3], int matrix[3][3])
{
    int r = 0, g = 0, b = 0;
    r = qRed(colors[0][0]) * matrix[0][0] + qRed(colors[0][1]) * matrix[0][1] + qRed(colors[0][2]) * matrix[0][2] +
            qRed(colors[1][0]) * matrix[1][0] + qRed(colors[1][1] )* matrix[1][1] + qRed(colors[1][2]) * matrix[1][2] +
            qRed(colors[2][0]) * matrix[2][0] + qRed(colors[2][1]) * matrix[2][1] + qRed(colors[2][2]) * matrix[2][2];
    g = qGreen(colors[0][0]) * matrix[0][0] + qGreen(colors[0][1]) * matrix[0][1] + qGreen(colors[0][2]) * matrix[0][2] +
            qGreen(colors[1][0]) * matrix[1][0] + qGreen(colors[1][1]) * matrix[1][1] + qGreen(colors[1][2]) * matrix[1][2] +
            qGreen(colors[2][0]) * matrix[2][0] + qGreen(colors[2][1]) * matrix[2][1] + qGreen(colors[2][2]) * matrix[2][2];
    b = qBlue(colors[0][0])* matrix[0][0] + qBlue(colors[0][1]) * matrix[0][1] + qBlue(colors[0][2]) * matrix[0][2] +
            qBlue(colors[1][0]) * matrix[1][0] + qBlue(colors[1][1]) * matrix[1][1] + qBlue(colors[1][2]) * matrix[1][2] +
            qBlue(colors[2][0]) * matrix[2][0] + qBlue(colors[2][1] )* matrix[2][1] + qBlue(colors[2][2]) * matrix[2][2];
    r = qMax (0,
              qMin (r, 255));
    g = qMax (0,
              qMin (g, 255));
    b = qMax (0,
              qMin (b, 255));
    return qRgb (r, g, b);
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

QRgb Graphic::colorNormir(QRgb colorX, QRgb colorY)
{
    double rx = qRed(colorX), gx = qGreen(colorX), bx = qBlue(colorX);
    double ry = qRed(colorY), gy = qGreen(colorY), by = qBlue(colorY);
    rx = sqrt(rx * rx + ry * ry);
    gx = sqrt(gx * gx + gy * gy);
    bx = sqrt(bx * bx + by * by);
    int r = qMax (0,
              qMin (static_cast <int> (rx), 255));
    int g = qMax (0,
              qMin (static_cast <int>(gx), 255));
    int b = qMax (0,
              qMin (static_cast <int>(bx), 255));
    return qRgb (r, g, b);
}

unsigned char Graphic::componentNormir(unsigned char colorX, unsigned char colorY)
{
    return static_cast <unsigned char> (qMax (0,
                 qMin (static_cast <int> (sqrt(colorX * colorX + colorY * colorY)), 255)));
}

void Graphic::sobelOperator()
{
    int w = image->width();
    int h = image->height();

    int sobelMaskY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int sobelMaskX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    QRgb *imageBytes[h];
    QRgb *newImageBytes[h];
    for (int i = 0; i < h; i++){
        imageBytes[i] = (QRgb*)(image->scanLine(i));
    }

    for (int i = 0; i < h; i++){
        newImageBytes[i] = new QRgb[w];
        for (int j = 0; j < w; j++){
            newImageBytes[i][j] = imageBytes[i][j];
        }
    }

    for (int i = 1; i < w-1; i++) {
        int kx = -1;        //обозначают границы
        int ky = -1;
        for (int j = 1; j < h-1; j++) {
            QRgb col[3][3];
            col[0][0] = newImageBytes[j - (ky < 0 ? 0 : 1)][i - (kx < 0 ? 0 : 1)];//newImage.pixelColor(i - (kx < 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
            col[0][1] = newImageBytes[j][i - (kx < 0 ? 0 : 1)];//newImage.pixelColor(i - (kx < 0 ? 0 : 1), j);
            col[0][2] = newImageBytes[j + (ky > 0 ? 0 : 1)][i - (kx < 0 ? 0 : 1)];//newImage.pixelColor(i - (kx < 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
            col[1][0] = newImageBytes[j][i];//newImage.pixelColor(i, j - (ky < 0 ? 0 : 1));
            col[1][1] = newImageBytes[j + (ky > 0 ? 0 : 1)][i];//newImage.pixelColor(i, j);
            col[1][2] = newImageBytes[j - (ky < 0 ? 0 : 1)][i];//newImage.pixelColor(i, j + (ky > 0 ? 0 : 1));
            col[2][0] = newImageBytes[j - (ky < 0 ? 0 : 1)][i + (kx > 0 ? 0 : 1)];//newImage.pixelColor(i + (kx > 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
            col[2][1] = newImageBytes[j][i + (kx > 0 ? 0 : 1)];//newImage.pixelColor(i + (kx > 0 ? 0 : 1), j);
            col[2][2] = newImageBytes[j + (ky > 0 ? 0 : 1)][i + (kx > 0 ? 0 : 1)];//newImage.pixelColor(i + (kx > 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
            QRgb color = colorNormir(matrixColorMul(col, sobelMaskX), matrixColorMul(col, sobelMaskY));


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
            imageBytes[j][i] = color;

        }
    }

//    QColor col[3][3];

//    QImage newImage = QImage (*image);

////    float r,g,b;
//    int kx, ky;     //обозначают границы
//    for (int i = 1; i < w-1; i++) {
//        kx = -1;
//        ky = -1;
//        for (int j = 1; j < h-1; j++) {
//            col[0][0] = newImage.pixelColor(i - (kx < 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
//            col[0][1] = newImage.pixelColor(i - (kx < 0 ? 0 : 1), j);
//            col[0][2] = newImage.pixelColor(i - (kx < 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
//            col[1][0] = newImage.pixelColor(i, j - (ky < 0 ? 0 : 1));
//            col[1][1] = newImage.pixelColor(i, j);
//            col[1][2] = newImage.pixelColor(i, j + (ky > 0 ? 0 : 1));
//            col[2][0] = newImage.pixelColor(i + (kx > 0 ? 0 : 1), j - (ky < 0 ? 0 : 1));
//            col[2][1] = newImage.pixelColor(i + (kx > 0 ? 0 : 1), j);
//            col[2][2] = newImage.pixelColor(i + (kx > 0 ? 0 : 1), j + (ky > 0 ? 0 : 1));
//            QColor color = colorNormir(matrixColorMul(col, sobelMaskX), matrixColorMul(col, sobelMaskY));

//            image->setPixelColor (i, j, color);

//            if (i == w - 1)
//                ky = 1;
//            else
//                ky = 0;
//            if (j == h - 1)
//                kx = 1;
//            else //if (j != 0)
//                    kx = 0;
////                else
////                    ky = -1;
//        }
//    }
}

QImage *Graphic::outlineSelectionLinear()
{
    qDebug()<<"Зашли в последовательный";
    setYUVMatix();
    setYUV();
    sobelOperator();
    Binarization();
    return image;
}

QImage *Graphic::outlineSelectionParallel(int threadCount)
{
    QList <QRect> rects;
    QList <ThreadGraphic*> threads;
    int lineW = image->width()/threadCount; //ширина одной полоски изображения
    int lineH = image->height();
//    Y = new unsigned char [image->width() * image->height()];
//    U = new unsigned char [image->width() * image->height()];
//    V = new unsigned char [image->width() * image->height()];
    ThreadGraphic *ptr;
    int x = 1;
    for (int i = 0; i < threadCount; i++){
        rects.append(QRect(x, 1, i == threadCount - 1 ? image->width() - x - 1  : lineW, lineH - 2));
        ptr = new ThreadGraphic();
        threads.append(ptr);
        threads[threads.count() - 1]->setImagePointer(image);
        threads[threads.count() - 1]->setRect(rects[rects.count() - 1]);
        threads[threads.count() - 1]->setYUVPointers(Y, U, V);
        threads[threads.count() - 1]->setLIMIT(LIMIT);
        x += lineW;
        threads[i]->run();
    }
    qDebug()<<"Зашли в параллельный";
//    for (int i = 0; i < threadCount; i++){
//        threads[i]->run();
//    }
    bool theadFinished = false;

    while (!theadFinished){        //пока все потоки не завершат работу
        theadFinished = true;
        for (int i = 0; i < threads.count(); i++){  //раз за разом обходим наши потоки
            if (threads[i]->isRunning()){
                theadFinished = false;
                break;
            }
        }

    }

////здесь будем склеивать
//    QSize ptrSize = image->size();
//    delete image;
//    image = new QImage (ptrSize, QImage::Format_RGB32);
//    QPainter p (image);
//    x = 1;
//    for (int i = 0; i < threadCount; i++){
//        p.drawImage(QRect(x, 0, i == threadCount - 1 ? image->width() - x  : lineW, lineH), threads[i]->getImagePart()->copy(QRect(1, 1, lineW - 1, lineH)));
//        x += lineW;
//        delete threads[i];
//    }
        for (int i = 0; i < threadCount; i++){
            delete threads[i];
        }
    threads.clear();
    return image;
}

QImage *Graphic::outlineSelectionOMP(int threadCount)
{
    qDebug()<<"Зашли в OMP";
    omp_set_num_threads(threadCount);
    setYUVMatixOMP();
    setYUVOMP();
    sobelOperatorOMP();
    BinarizationOMP();
    return image;
}

QImage *Graphic::outlineSelectionMPI(int threadCount)
{
    int procRank, procSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);   //получаем номер процесса
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    if (procRank == 0){
        qDebug()<<"Зашли в MPI";
        qDebug()<<procSize;

        width = image->width();
        height = image->height();
        // RGB32 to YUV420
        int size = width * height;
        // Y
        Y = new unsigned char [size];
        U = new unsigned char [size];
        V = new unsigned char [size];
    }

    MPI_Barrier(MPI_COMM_WORLD);    //барьер, чтобы закончить подготовку

    setYUVMatixMPI();
    MPI_Barrier(MPI_COMM_WORLD);    //барьер, чтобы все потоки закончили
    setYUVMPI();
    MPI_Barrier(MPI_COMM_WORLD);    //барьер, чтобы все потоки закончили
    sobelOperatorMPI();
    MPI_Barrier(MPI_COMM_WORLD);    //барьер, чтобы все потоки закончили
    BinarizationMPI();
    MPI_Barrier(MPI_COMM_WORLD);    //барьер, чтобы все потоки закончили
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

