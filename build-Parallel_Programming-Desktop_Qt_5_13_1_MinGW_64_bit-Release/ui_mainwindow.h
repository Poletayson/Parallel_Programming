/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *open;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_6;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_5;
    QVBoxLayout *verticalLayout_7;
    QSlider *horizontalSlider;
    QVBoxLayout *verticalLayout_2;
    QPushButton *ButtonNeg;
    QPushButton *ButtonBinary;
    QPushButton *ButtonGray;
    QPushButton *ButtonRepair;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_3;
    QPushButton *ButtonNoise;
    QPushButton *ButtonMedian;
    QPushButton *ButtonGauss;
    QPushButton *ButtonRezk;
    QVBoxLayout *verticalLayout_8;
    QPushButton *pushButton;
    QPushButton *ButtonAquarel;
    QPushButton *ButtonMy;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(698, 548);
        open = new QAction(MainWindow);
        open->setObjectName(QString::fromUtf8("open"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

        gridLayout_2->addLayout(verticalLayout_4, 1, 7, 1, 1);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        radioButton = new QRadioButton(centralWidget);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setChecked(true);

        verticalLayout_6->addWidget(radioButton);

        radioButton_2 = new QRadioButton(centralWidget);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        verticalLayout_6->addWidget(radioButton_2);

        radioButton_3 = new QRadioButton(centralWidget);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        verticalLayout_6->addWidget(radioButton_3);

        radioButton_4 = new QRadioButton(centralWidget);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));

        verticalLayout_6->addWidget(radioButton_4);

        radioButton_5 = new QRadioButton(centralWidget);
        radioButton_5->setObjectName(QString::fromUtf8("radioButton_5"));

        verticalLayout_6->addWidget(radioButton_5);


        gridLayout_2->addLayout(verticalLayout_6, 1, 6, 1, 1);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMouseTracking(true);
        horizontalSlider->setMinimum(-255);
        horizontalSlider->setMaximum(255);
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout_7->addWidget(horizontalSlider);


        gridLayout_2->addLayout(verticalLayout_7, 1, 5, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        ButtonNeg = new QPushButton(centralWidget);
        ButtonNeg->setObjectName(QString::fromUtf8("ButtonNeg"));

        verticalLayout_2->addWidget(ButtonNeg);

        ButtonBinary = new QPushButton(centralWidget);
        ButtonBinary->setObjectName(QString::fromUtf8("ButtonBinary"));

        verticalLayout_2->addWidget(ButtonBinary);

        ButtonGray = new QPushButton(centralWidget);
        ButtonGray->setObjectName(QString::fromUtf8("ButtonGray"));

        verticalLayout_2->addWidget(ButtonGray);

        ButtonRepair = new QPushButton(centralWidget);
        ButtonRepair->setObjectName(QString::fromUtf8("ButtonRepair"));

        verticalLayout_2->addWidget(ButtonRepair);


        gridLayout_2->addLayout(verticalLayout_2, 1, 0, 1, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));

        gridLayout_2->addLayout(verticalLayout_5, 1, 3, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        ButtonNoise = new QPushButton(centralWidget);
        ButtonNoise->setObjectName(QString::fromUtf8("ButtonNoise"));
        ButtonNoise->setEnabled(true);

        verticalLayout_3->addWidget(ButtonNoise);

        ButtonMedian = new QPushButton(centralWidget);
        ButtonMedian->setObjectName(QString::fromUtf8("ButtonMedian"));
        ButtonMedian->setEnabled(true);

        verticalLayout_3->addWidget(ButtonMedian);

        ButtonGauss = new QPushButton(centralWidget);
        ButtonGauss->setObjectName(QString::fromUtf8("ButtonGauss"));

        verticalLayout_3->addWidget(ButtonGauss);

        ButtonRezk = new QPushButton(centralWidget);
        ButtonRezk->setObjectName(QString::fromUtf8("ButtonRezk"));

        verticalLayout_3->addWidget(ButtonRezk);


        gridLayout_2->addLayout(verticalLayout_3, 1, 1, 1, 1);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        verticalLayout_8->addWidget(pushButton);

        ButtonAquarel = new QPushButton(centralWidget);
        ButtonAquarel->setObjectName(QString::fromUtf8("ButtonAquarel"));

        verticalLayout_8->addWidget(ButtonAquarel);

        ButtonMy = new QPushButton(centralWidget);
        ButtonMy->setObjectName(QString::fromUtf8("ButtonMy"));

        verticalLayout_8->addWidget(ButtonMy);


        gridLayout_2->addLayout(verticalLayout_8, 1, 4, 1, 1);


        gridLayout->addLayout(gridLayout_2, 2, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 698, 20));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(open);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        open->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
        radioButton->setText(QCoreApplication::translate("MainWindow", "\320\257\321\200\320\272\320\276\321\201\321\202\321\214", nullptr));
        radioButton_2->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\320\275\321\202\321\200\320\260\321\201\321\202", nullptr));
        radioButton_3->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\321\200\320\276\320\263 \320\261\320\270\320\275\320\260\321\200\320\270\320\267\320\260\321\206\320\270\320\270", nullptr));
        radioButton_4->setText(QCoreApplication::translate("MainWindow", "\320\243\321\200\320\276\320\262\320\265\320\275\321\214 \321\210\321\203\320\274\320\260", nullptr));
        radioButton_5->setText(QCoreApplication::translate("MainWindow", "\320\232", nullptr));
        ButtonNeg->setText(QCoreApplication::translate("MainWindow", "\320\235\320\265\320\263\320\260\321\202\320\270\320\262", nullptr));
        ButtonBinary->setText(QCoreApplication::translate("MainWindow", "\320\221\320\270\320\275\320\260\321\200\320\270\320\267\320\260\321\206\320\270\321\217", nullptr));
        ButtonGray->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\321\202\320\265\320\275\320\272\320\270 \321\201\320\265\321\200\320\276\320\263\320\276", nullptr));
        ButtonRepair->setText(QCoreApplication::translate("MainWindow", "\320\241\320\261\321\200\320\276\321\201", nullptr));
        ButtonNoise->setText(QCoreApplication::translate("MainWindow", "\320\250\321\203\320\274", nullptr));
        ButtonMedian->setText(QCoreApplication::translate("MainWindow", "\320\234\320\265\320\264\320\270\320\260\320\275\320\275. \321\204.", nullptr));
        ButtonGauss->setText(QCoreApplication::translate("MainWindow", "\320\244. \320\223\320\260\321\203\321\201\321\201\320\260", nullptr));
        ButtonRezk->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\262\321\213\321\210. \321\200\320\265\320\267\320\272\320\276\321\201\321\202\320\270", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "YUV", nullptr));
        ButtonAquarel->setText(QCoreApplication::translate("MainWindow", "\320\220\320\272\320\262\320\260\321\200\320\265\320\273\321\214", nullptr));
        ButtonMy->setText(QCoreApplication::translate("MainWindow", "\320\234\320\276\320\271 \321\204\320\270\320\273\321\214\321\202\321\200", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
