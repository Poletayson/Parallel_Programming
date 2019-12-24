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

    ui->horizontalLayout->addWidget(myGraphic);
    ui->horizontalLayout->addWidget(myGraphic2);

    random = new QRandomGenerator ();

   connect(ui->open, SIGNAL(triggered(bool)), this, SLOT(getFile()));
   connect(ui->ButtonRepair, SIGNAL(clicked()), this, SLOT(Repair ()));
//   connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(sliderOb()));
   connect(ui->ButtonNoise, SIGNAL(clicked()), this, SLOT(setNoise ()));
//   connect(ui->ButtonMedian, SIGNAL(clicked()), this, SLOT(Median ()));
   //connect(ui->ButtonGauss, SIGNAL(clicked()), this, SLOT(Gauss ()));

   myGraphic2->setLIMIT(ui->horizontalSlider->value());
//    ui->verticalLayout->addWidget(myGraphic);
//    ui->verticalLayout_4->addWidget(myGraphic2);

    //myGraphic->Drawing();
   ui->ButtonRezk->hide();
   ui->ButtonGauss->hide();
   ui->ButtonNoise->hide();
   ui->ButtonRepair->hide();
   ui->ButtonAquarel->hide();
   ui->ButtonMy->hide();
   ui->pushButton_2->hide();

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

///
//        myGraphic2->setImage(myGraphic->getImage());
//        QDateTime start2 = QDateTime::currentDateTime();

//        myGraphic2->outlineSelectionParallel(ui->spinBoxThreadCount->value());//sobelOperator();

//        QDateTime finish2 = QDateTime::currentDateTime();

////        delete myGraphic2->imageItem;
////        myGraphic2->imageItem = myGraphic2->myScene->addPixmap(QPixmap::fromImage(*image));
//        secs = start2.msecsTo(finish2);
//        ui->labelParell->setText("Параллельный алгоритм: " + QString::number(secs));

///
        myGraphic2->setImage(myGraphic->getImage());
        QDateTime start2 = QDateTime::currentDateTime();
        delete image;
        image = myGraphic2->outlineSelectionOMP(ui->spinBoxThreadCount->value());//sobelOperator();

        QDateTime finish2 = QDateTime::currentDateTime();

        delete myGraphic2->imageItem;
        myGraphic2->imageItem = myGraphic2->myScene->addPixmap(QPixmap::fromImage(*image));
        secs = start2.msecsTo(finish2);
        ui->labelOpenMP->setText("OpenMP-алгоритм: " + QString::number(secs));

///
        myGraphic2->setImage(myGraphic->getImage());
        QDateTime start3 = QDateTime::currentDateTime();
        delete image;
        image = myGraphic2->outlineSelectionMPI(ui->spinBoxThreadCount->value());//sobelOperator();

        QDateTime finish3 = QDateTime::currentDateTime();

        delete myGraphic2->imageItem;
        myGraphic2->imageItem = myGraphic2->myScene->addPixmap(QPixmap::fromImage(*image));
        secs = start3.msecsTo(finish3);
        ui->labelParell->setText("MPI-алгоритм: " + QString::number(secs));
}

