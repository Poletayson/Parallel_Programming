#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <graphic.h>
#include <mainwindow.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    Dialog(Graphic *gr, QWidget *parent);
    ~Dialog();
    int func;

private:
    Ui::Dialog *ui;
    //QGraphicsPixmapItem *reserveA;
    //QGraphicsPixmapItem *reserveB;
    //QGraphicsPixmapItem *reserveBin;
    Graphic *myGr;

public slots:
    QPixmap* OK ();
    void Brightness ();
    void Contrast ();
    void Balance ();
};

#endif // DIALOG_H
