#include "graphic.h"

Graphic::Graphic(QObject *parent) : QObject(parent)
{


    SIGMA = 0.8;
    //connect(this, SIGNAL (ResizeSignal()), this, SLOT(Drawing()));
}

void Graphic::GetFileImage (QString file)
{
//    if (!QFile::exists(file))
//        qDebug()<<"FILE_NOT_FOUND! "<<file;
    image = new QImage (file);                        //картинка
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
    int width = imagePart->width();
    int height = imagePart->height();

    if(imagePart->format()!=QImage::Format_RGB32    && imagePart->format() != QImage::Format_ARGB32)
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
         tempColor = imagePart->pixelColor(i, j);//Canvas->Pixels[i][j];
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
    if (imagePart != nullptr)
    {
        int procRank, procSize;
        MPI_Comm_rank(MPI_COMM_WORLD, &procRank);   //получаем номер процесса
        MPI_Comm_size(MPI_COMM_WORLD, &procSize);


        //int x1 = width/procSize * procRank; //начало
        //if (x1 == 0) ++x1;
        //int lineH = height;
        //int x2 = (procRank != procSize - 1) ? x1 + width/procSize : width;

        int w = imagePart->width();
        int h = imagePart->height();

        QRgb *imageBytes[h];
        for (int i = 0; i < h; i++){
            imageBytes[i] = (QRgb*)(imagePart->scanLine(i));
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
    int w = imagePart->width();
    int h = imagePart->height();

    int sobelMaskY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int sobelMaskX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    QRgb *imageBytes[h];
    QRgb *newImageBytes[h];
    for (int i = 0; i < h; i++){
        imageBytes[i] = (QRgb*)(imagePart->scanLine(i));
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
}

void Graphic::BinarizationMPI()
{
    int w = imagePart->width();
    int h = imagePart->height();

    QRgb *imageBytes[h];
    for (int i = 0; i < h; i++){
        imageBytes[i] = (QRgb*)(imagePart->scanLine(i));
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
    //qDebug()<<"Зашли в последовательный";
    setYUVMatix();
    setYUV();
    sobelOperator();
    Binarization();
    return image;
}


QImage *Graphic::outlineSelectionOMP(int threadCount)
{
    //qDebug()<<"Зашли в OMP";
    omp_set_num_threads(threadCount);
    setYUVMatixOMP();
    setYUVOMP();
    sobelOperatorOMP();
    BinarizationOMP();
    return image;
}

QImage *Graphic::outlineSelectionMPI(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    double t1 = MPI_Wtime();
    int procRank, procSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);   //получаем номер процесса
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);

    if (procRank == 0){
        std::cout<<"ThreadCount: "<<procSize<<std::endl;
        width = image->width();
        height = image->height();

        int lineW = width/procSize; //ширина одной полоски изображения
        int x = 0;
//        x1 = x;
//        x2 = (procSize == 1) ? width - 2  : x + lineW;


        imagePart = new QImage (image->copy(x, 0, (procSize == 1) ? width : lineW + 2, height));  //копируем кусок
//        std::cout<<"procRank = "<< procRank<<", w = "<< imagePart->width()<<", h = "<< imagePart->height()<<"\n";
//        std::cout.flush();

        // RGB32 to YUV420
//        int size = width * height;
//        // Y
//        Y = new unsigned char [size];
//        U = new unsigned char [size];
//        V = new unsigned char [size];

        for (int i = 1; i < procSize; i++){
            QImage imagePtr;
            x += lineW;

            imagePtr = image->copy(x - 2, 0, (i == procSize - 1) ? width - x : lineW + 2, height);

            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);

            out << imagePtr;
            out.device()->seek(0);

            //отправляем x1
            int size = block.size();

//            std::cout<<size<<" size send\n";
//            std::cout.flush();
            MPI_Send(&size,
                     1,
                     MPI_INT,
                     i,         //номер процесса
                     0,     //тег x1
                     MPI_COMM_WORLD);

            char *bf = block.data();
            //отправляем кусок
            MPI_Send(bf,
                     size,
                     MPI_BYTE, //MPI_BYTE
                     i,         //номер процесса
                     3,     //тег самого содержимого
                     MPI_COMM_WORLD);
        }
    }

    //отправили

    if (procRank != 0){
        int size;   //принимаем размер
        MPI_Recv(&size,
                 1,
                 MPI_INT,
                 0,         //номер процесса
                 0,     //тег x1
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);


//будем принимать кусок
        char *bf = new char[size];
        MPI_Recv(bf,
                 size,
                 MPI_BYTE,
                 0,         //номер процесса
                 3,     //тег изображения
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        QByteArray *ba = new QByteArray();
        ba->append(bf, size);   //наполняем
        QDataStream in(*ba);
//        std::cout<<"procRank = "<< procRank<<", baS = "<< ba->size()<<", S = "<< size<<"\n";
//        std::cout.flush();

        QImage im;
        in >> im;
        imagePart = new QImage(im);
        //новое изображение

//        std::cout<<"procRank = "<< procRank<<", wPart = "<< imagePart->width()<<", hPart = "<< imagePart->height()<<"\n";
//        std::cout.flush();
    }
    setYUVMatixMPI();
    setYUVMPI();
    sobelOperatorMPI();
    BinarizationMPI();
//    std::cout<<"BinarizationMPI_Finished\n";
//    std::cout.flush();
//теперь нужно сшить
    //Здесь будем отправлять
    if (procRank != 0){

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
//        std::cout<<"procRank = "<< procRank<<", wPartAA = "<< imagePart->width()<<", hPartAA = "<< imagePart->height()<<"\n";
//        std::cout.flush();
        QImage ptr = *imagePart;
        out << ptr;
        out.device()->seek(0);

        int size = block.size();
//        std::cout<<size<<" size sendToMain\n";
//        std::cout.flush();

        MPI_Send(&size,
                 1,
                 MPI_INT,
                 0,         //номер процесса
                 4,     //тег size
                 MPI_COMM_WORLD);

        char *bf = block.data();
        //отправляем кусок
        MPI_Send(bf,
                 size,
                 MPI_BYTE, //MPI_BYTE
                 0,         //номер процесса
                 5,     //тег самого содержимого
                 MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);    //барьер, чтобы все потоки закончили

    //Здесь будем принимать и сшивать
    if (procRank == 0){
        QList<QImage *> imlist;
        imlist << imagePart;
        int w = imagePart->width();

        for (int i = 1; i < procSize; i++){
            int size;   //принимаем размер
            MPI_Recv(&size,
                     1,
                     MPI_INT,
                     i,         //номер процесса
                     4,     //тег x1
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//            std::cout<<size<<" recStart_size\n";
//            std::cout.flush();

            //будем принимать кусок
            char *bf = new char[size];
            MPI_Recv(bf,
                     size,
                     MPI_BYTE,
                     i,         //номер процесса
                     5,     //тег изображения
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            QByteArray *ba = new QByteArray();
            ba->append(bf, size);   //наполняем
            QDataStream in(*ba);
            //        std::cout<<"procRank = "<< procRank<<", baS = "<< ba->size()<<", S = "<< size<<"\n";
            //        std::cout.flush();

            QImage im;
            in >> im;
            imlist << new QImage(im);
            w += im.width();
        }
        //новое изображение
        QImage *newIm = new  QImage (w, height, QImage::Format_ARGB32);
        QPainter p(newIm);
        int x = 0;
        for (int i = 0; i < imlist.count(); i++){
            //p.setCompositionMode(QPainter::co mode);
            p.drawImage(x - 1, 0, imlist[i]->copy(1, 0, imlist[i]->width() - 1, imlist[i]->height()));
            x += imlist[i]->width() - 1;
        }
        double t2 = MPI_Wtime();
        std::cout<<"Time_MPI: "<<t2 - t1<<std::endl;
        std::cout.flush();
        image = newIm;
    }
    MPI_Finalize();

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


