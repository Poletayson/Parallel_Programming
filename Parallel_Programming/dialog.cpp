#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //ui->buttonBox->
}

Dialog::Dialog(Graphic *gr, QWidget *parent):
    QDialog(parent),
    ui(new Ui::Dialog)

{


    myGr = gr;
//    myGr->reserve = gr->reserve;
//    myGr->image = new QGraphicsPixmapItem (myGr->reserve->pixmap());
//    reserveA = new QGraphicsPixmapItem (myGr->reserve->pixmap());
//    ui->verticalLayout->addWidget(myGr);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(getFile()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(Balance()));

}

Dialog::~Dialog()
{
    delete ui;
}

QPixmap* Dialog::OK ()
{

}


void Dialog::Brightness()
{
    if (myGr->reserve != nullptr)
    {
        int brPl = ui->horizontalSlider->value();
        if (myGr->imageItem != nullptr)
            delete myGr->imageItem;
        myGr->imageItem = myGr->myScene->addPixmap(*myGr->Brightness(brPl));
    }
}
void Dialog::Contrast ()
{
    if (myGr->reserve != nullptr)
    {
        int brPl = ui->horizontalSlider->value();
        if (myGr->imageItem != nullptr)
            delete myGr->imageItem;
        myGr->imageItem = myGr->myScene->addPixmap(*myGr->Brightness(brPl));
    }
}
