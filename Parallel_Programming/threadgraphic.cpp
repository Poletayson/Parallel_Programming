#include "threadgraphic.h"

ThreadGraphic::ThreadGraphic(QObject *parent) : QThread(parent)
{

}

void ThreadGraphic::setImagePointer(QImage *value)
{
    image = value;
}

void ThreadGraphic::setYUVPointers(unsigned char *Yp, unsigned char *Up, unsigned char *Vp)
{
    Y = Yp;
    U = Up;
    V = Vp;
}

void ThreadGraphic::setRect(const QRect &value)
{
    rect = value;
}

bool ThreadGraphic::setYUVMatix()
{
    int width = rect.x() + rect.width();
    int height = rect.height();

    if(image->format()!=QImage::Format_RGB32    && image->format() != QImage::Format_ARGB32)
    {
       printf("Wrong image format\n");
       return false;
    }

    // RGB32 to YUV420
//    int size = width * height;
    // Y
//    Y = new unsigned char [size];
//    U = new unsigned char [size];
//    V = new unsigned char [size];

    QColor tempColor;
    for (int i = rect.x(); i < width; i++)
       for (int j = rect.y(); j < height; j++)
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

bool ThreadGraphic::setYUV()
{
    if (image != nullptr)
    {
        QColor* col = new QColor ();
        int w = rect.x() + rect.width();
        int h = rect.height();
        for (int i = rect.x(); i < w; i++)
            for (int j = rect.y(); j < h; j++)
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

void ThreadGraphic::sobelOperator()
{
    int sobelMaskY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int sobelMaskX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};


    QColor col[3][3];

    QImage newImage = QImage (*image);
    int w = rect.x() + rect.width();
    int allw = image->width();
    int h = image->height();
//    float r,g,b;
    int kx = 0, ky = 0;     //обозначают границы
    for (int i = rect.x(); i < w; i++) {
        if (i == 0) kx = -1;
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
}

void ThreadGraphic::run()
{
    setYUVMatix();
    setYUV();
    sobelOperator();
    binarization();
}

QColor ThreadGraphic::matrixColorMul(QColor colors[3][3], int matrix[3][3])
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

unsigned char ThreadGraphic::matrixMul(unsigned char channel[3][3], int matrix[3][3])
{
    int c = 0;
    c = channel[0][0] * matrix[0][0] + channel[0][1] * matrix[0][1] + channel[0][2] * matrix[0][2] +
            channel[1][0] * matrix[1][0] + channel[1][1] * matrix[1][1] + channel[1][2] * matrix[1][2] +
            channel[2][0] * matrix[2][0] + channel[2][1] * matrix[2][1] + channel[2][2] * matrix[2][2];
    c = qMax (0,
              qMin (c, 255));
    return c;
}

QColor ThreadGraphic::colorNormir(QColor colorX, QColor colorY)
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

unsigned char ThreadGraphic::componentNormir(unsigned char colorX, unsigned char colorY)
{
    return static_cast <unsigned char> (qMax (0,
                                              qMin (static_cast <int> (sqrt(colorX * colorX + colorY * colorY)), 255)));
}

void ThreadGraphic::binarization()
{
        QColor col;
        int bright;           //яркость
        int w = rect.x() + rect.width();
//        int allw = image->width();
        int h = image->height();
        for (int i = rect.x(); i < w; i++)
            for (int j = 0; j < h; j++)
            {
                col = image->pixelColor(i, j);
                bright = static_cast<int> (0.299*col.red() + 0.5876*col.green() + 0.114*col.blue());

                if (bright > LIMIT)       //ярость больше порога?
                {
                    col.setBlue(255);
                    col.setGreen(255);
                    col.setRed(255);
                }
                else
                {
                    col.setBlue(0);
                    col.setGreen(0);
                    col.setRed(0);
                }
                image->setPixelColor(i, j, col);
            }
}


