#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfile.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer ();

    myGraphic = new Graphic ();
    myGraphic2 = new Graphic ();
    histogram = new Graphic ();
    histogram->setAlignment(Qt::AlignCenter);        //выравнивание по центру
    histogram->setMaximumWidth(260);
    histogram->setMaximumHeight(150);
    //histogram->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //histogram->setsi
    ui->horizontalLayout->addWidget(myGraphic);
    ui->horizontalLayout->addWidget(myGraphic2);
    ui->verticalLayout_4->addWidget(histogram);

    random = new QRandomGenerator ();

   connect(ui->open, SIGNAL(triggered(bool)), this, SLOT(getFile()));
   connect(ui->ButtonRepair, SIGNAL(clicked()), this, SLOT(Repair ()));
//   connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(sliderOb()));
   connect(ui->ButtonNoise, SIGNAL(clicked()), this, SLOT(setNoise ()));
//   connect(ui->ButtonMedian, SIGNAL(clicked()), this, SLOT(Median ()));
   connect(ui->ButtonGauss, SIGNAL(clicked()), this, SLOT(Gauss ()));

//    ui->verticalLayout->addWidget(myGraphic);
//    ui->verticalLayout_4->addWidget(myGraphic2);

    //myGraphic->Drawing();
   ui->ButtonRezk->hide();
   ui->ButtonGauss->hide();
   ui->ButtonNoise->hide();
   image = new QImage ();
}



MainWindow::~MainWindow()
{

    delete ui;
}



QString MainWindow::getFile()             //выбираем список файлов
{
    #include <QDir>
    QString path = QApplication::applicationDirPath();  //QDir::currentPath();      //текущая директория
    if (QDir(path + "/Input").exists())
         path = path + "/Input";
    else
        QDir(path).mkdir("Input");
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, "Выберите входной файл", path);
    //qDebug() << "Взято " << stl.count() << " файлов";
    //получили файл
    if (! fileName.isEmpty()){
        myGraphic->GetFileImage(fileName);      //получаем изображение
        setStart ();
    }
/////
//    buttGr->button(0)->setChecked(true);
//    buttonChange ();
////
    //ui->radioButton->setChecked(true);
    //ui->horizontalSlider->setValue(0);
    return fileName;

}

void MainWindow::Repair ()      //восстановление
{
//    if (myGraphic->image != Q_NULLPTR)
//        delete myGraphic->image;
//    if (myGraphic->reserve != Q_NULLPTR)
//        delete myGraphic->reserve;

    if (myGraphic2->imageItem != Q_NULLPTR)
        delete myGraphic2->imageItem;
    myGraphic2->imageItem = myGraphic2->myScene->addPixmap(myGraphic->imageItem->pixmap());


    if (myGraphic2->reserve != Q_NULLPTR)
        delete myGraphic2->reserve;
    myGraphic2->reserve = new QGraphicsPixmapItem (myGraphic->imageItem->pixmap());
    makeHist ();
}


void MainWindow::setStart ()
{
    if (myGraphic2->imageItem != nullptr)
        delete myGraphic2->imageItem;
    myGraphic2->imageItem = myGraphic2->myScene->addPixmap(myGraphic->imageItem->pixmap());
    myGraphic2->myScene->setSceneRect(0, 0, myGraphic2->imageItem->pixmap().width(), myGraphic2->imageItem->pixmap().height());
    if (myGraphic2->reserve != nullptr)
        delete myGraphic2->reserve;
    myGraphic2->reserve = new QGraphicsPixmapItem (myGraphic2->imageItem->pixmap());
    makeHist ();
}


//void MainWindow::Binarization ()
//{
//    if (myGraphic2->reserve != nullptr)
//    {
//        int brPl = ui->horizontalSlider->value();
//        if (myGraphic2->imageItem != nullptr)
//            delete myGraphic2->imageItem;
//        myGraphic2->imageItem = myGraphic2->myScene->addPixmap(*myGraphic2->Binarization(brPl));
//        makeHist ();
//    }
//}



void MainWindow::makeHist ()
{
    int brigtLevels [256];
    for (int i = 0; i < 256; i++) {
        brigtLevels [i] = 0;
    }
    QColor* col;
    qreal bright;           //яркость
    QImage *newImage = new QImage (myGraphic2->imageItem->pixmap().toImage());
    for (int i = 0; i < myGraphic2->imageItem->pixmap().width(); i++)
        for (int j = 0; j < myGraphic2->imageItem->pixmap().height(); j++)
        {
            col = new QColor (newImage->pixelColor(i, j));
            bright = 0.289*col->red() + 0.5556*col->green() + 0.112*col->blue();    //яркость
            brigtLevels[(int)bright]++;     //добавляем к яркости
            delete col;
        }
    delete newImage;
//получили массив яркостей
    QPen p (Qt::black);
    int max = brigtLevels[0];
    for (int i = 0; i < 256; i++) {
        if (brigtLevels[i] > max)
            max = brigtLevels[i];
    }

    //histogram->myScene = new QGraphicsScene ();
    histogram->myScene->setSceneRect(0, 0, 256, histogram->height() - 3);   //размер сцены под картинку
    histogram->myScene->clear();
    qreal m = histogram->myScene->height()/max;
    for (int i = 0; i < 256; i++) {
        histogram->scene()->addLine(i, histogram->height() - m*brigtLevels[i], i, histogram->height(), p);
    }
}

void MainWindow::setNoise ()
{
    if (myGraphic2->imageItem != Q_NULLPTR)
    {
        int noisePers = ui->horizontalSlider->value();      //процент шума
        int r, g, b;
        QImage *newImage = new QImage (myGraphic2->imageItem->pixmap().toImage());
        //QColor* col;
        QRgb c;
        for (int i = 0; i < myGraphic2->imageItem->pixmap().width(); i++)
            for (int j = 0; j < myGraphic2->imageItem->pixmap().height(); j++)
            {
                if (random->generate() % 100 < noisePers)
                {
                    r = random->generate() % 256;
                    g = random->generate() % 256;
                    b = random->generate() % 256;
                    c = qRgb(r, g, b);
//                    col = new QColor ();
//                    col->setBlue(b);
//                    col->setGreen(g);
//                    col->setRed(r);
                    newImage->setPixel (i, j, c); //Color(i, j, *col);
                    //delete col;
                }
            }
        delete myGraphic2->imageItem;
        myGraphic2->imageItem = myGraphic2->myScene->addPixmap (*new QPixmap (QPixmap::fromImage(*newImage)));
        delete myGraphic2->reserve;
        myGraphic2->reserve = new QGraphicsPixmapItem (myGraphic2->imageItem->pixmap());
        delete newImage;

    }
}
void MainWindow::Median()
{
    if (myGraphic2->reserve != nullptr)
    {
        if (myGraphic2->imageItem != nullptr)
            delete myGraphic2->imageItem;
        myGraphic2->imageItem = myGraphic2->myScene->addPixmap(*myGraphic2->Median());
        delete myGraphic->reserve;      //удаляем старый резерв и делаем текущее изображение резервным
        myGraphic2->reserve = new QGraphicsPixmapItem (myGraphic2->imageItem->pixmap());
        makeHist ();
    }
}

void MainWindow::Gauss()
{
    if (myGraphic2->reserve != nullptr)
    {
        if (myGraphic2->imageItem != nullptr)
            delete myGraphic2->imageItem;
        myGraphic2->imageItem = myGraphic2->myScene->addPixmap(*myGraphic2->Gauss());
        delete myGraphic->reserve;      //удаляем старый резерв и делаем текущее изображение резервным
        myGraphic2->reserve = new QGraphicsPixmapItem (myGraphic2->imageItem->pixmap());
        makeHist ();
    }
}


bool MainWindow::setYUVMatix()
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

bool MainWindow::setYUV()
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




void MainWindow::on_ButtonAquarel_clicked()
{
    myGraphic2->setImage(myGraphic->getImage());
//    image = myGraphic->getImage();
    myGraphic2->setYUVMatix();
    //myGraphic2->setYUV();

    //delete image;
    image = myGraphic2->sobelOperatorOneChannel(myGraphic2->getY());
    delete myGraphic2->imageItem;
    myGraphic2->imageItem = myGraphic2->myScene->addPixmap(QPixmap::fromImage(*image));
}

void MainWindow::on_ButtonMy_clicked()
{
    myGraphic2->setImage(myGraphic->getImage());
//    image = myGraphic->getImage();
    myGraphic2->setYUVMatix();

    image = myGraphic2->sobelOperatorOneChannel(myGraphic2->getU());
    delete myGraphic2->imageItem;
    myGraphic2->imageItem = myGraphic2->myScene->addPixmap(QPixmap::fromImage(*image));
}

void MainWindow::on_pushButton_2_clicked()
{
    myGraphic2->setImage(myGraphic->getImage());
    myGraphic2->setYUVMatix();

    //delete image;
    image = myGraphic2->sobelOperatorOneChannel(myGraphic2->getV());
    delete myGraphic2->imageItem;
    myGraphic2->imageItem = myGraphic2->myScene->addPixmap(QPixmap::fromImage(*image));
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    myGraphic2->setLIMIT(value);
}



void MainWindow::on_pushButtonYUV_clicked()
{
        myGraphic2->setImage(myGraphic->getImage());

        QDateTime start = QDateTime::currentDateTime();

        delete image;
        image = myGraphic2->outlineSelectionLinear();//sobelOperator();

        QDateTime finish = QDateTime::currentDateTime();


        delete myGraphic2->imageItem;
        myGraphic2->imageItem = myGraphic2->myScene->addPixmap(QPixmap::fromImage(*image));
        qint64 secs = start.msecsTo(finish);
        ui->labelLine->setText("Линейный алгоритм: " + QString::number(secs));
        //myGraphic2->ClearItem(myGraphic2->imageItem);
}

