/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
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
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
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
    QVBoxLayout *verticalLayout_3;
    QLabel *labelLine;
    QLabel *labelParell;
    QLabel *labelOpenMP;
    QSpinBox *spinBoxThreadCount;
    QPushButton *ButtonNoise;
    QPushButton *ButtonGauss;
    QPushButton *ButtonRezk;
    QVBoxLayout *verticalLayout_8;
    QPushButton *pushButtonYUV;
    QPushButton *ButtonAquarel;
    QPushButton *ButtonMy;
    QPushButton *pushButton_2;
    QPushButton *ButtonRepair;
    QVBoxLayout *verticalLayout_7;
    QSlider *horizontalSlider;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_5;
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
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        labelLine = new QLabel(centralWidget);
        labelLine->setObjectName(QString::fromUtf8("labelLine"));

        verticalLayout_3->addWidget(labelLine);

        labelParell = new QLabel(centralWidget);
        labelParell->setObjectName(QString::fromUtf8("labelParell"));

        verticalLayout_3->addWidget(labelParell);

        labelOpenMP = new QLabel(centralWidget);
        labelOpenMP->setObjectName(QString::fromUtf8("labelOpenMP"));

        verticalLayout_3->addWidget(labelOpenMP);

        spinBoxThreadCount = new QSpinBox(centralWidget);
        spinBoxThreadCount->setObjectName(QString::fromUtf8("spinBoxThreadCount"));
        spinBoxThreadCount->setMinimum(1);
        spinBoxThreadCount->setMaximum(50);

        verticalLayout_3->addWidget(spinBoxThreadCount);

        ButtonNoise = new QPushButton(centralWidget);
        ButtonNoise->setObjectName(QString::fromUtf8("ButtonNoise"));
        ButtonNoise->setEnabled(true);

        verticalLayout_3->addWidget(ButtonNoise);

        ButtonGauss = new QPushButton(centralWidget);
        ButtonGauss->setObjectName(QString::fromUtf8("ButtonGauss"));

        verticalLayout_3->addWidget(ButtonGauss);

        ButtonRezk = new QPushButton(centralWidget);
        ButtonRezk->setObjectName(QString::fromUtf8("ButtonRezk"));

        verticalLayout_3->addWidget(ButtonRezk);


        gridLayout_2->addLayout(verticalLayout_3, 1, 0, 1, 1);

        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        pushButtonYUV = new QPushButton(centralWidget);
        pushButtonYUV->setObjectName(QString::fromUtf8("pushButtonYUV"));

        verticalLayout_8->addWidget(pushButtonYUV);

        ButtonAquarel = new QPushButton(centralWidget);
        ButtonAquarel->setObjectName(QString::fromUtf8("ButtonAquarel"));

        verticalLayout_8->addWidget(ButtonAquarel);

        ButtonMy = new QPushButton(centralWidget);
        ButtonMy->setObjectName(QString::fromUtf8("ButtonMy"));

        verticalLayout_8->addWidget(ButtonMy);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        verticalLayout_8->addWidget(pushButton_2);

        ButtonRepair = new QPushButton(centralWidget);
        ButtonRepair->setObjectName(QString::fromUtf8("ButtonRepair"));

        verticalLayout_8->addWidget(ButtonRepair);


        gridLayout_2->addLayout(verticalLayout_8, 1, 3, 1, 1);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setMouseTracking(true);
        horizontalSlider->setMinimum(0);
        horizontalSlider->setMaximum(255);
        horizontalSlider->setValue(64);
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout_7->addWidget(horizontalSlider);


        gridLayout_2->addLayout(verticalLayout_7, 1, 4, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

        gridLayout_2->addLayout(verticalLayout_4, 1, 5, 1, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));

        gridLayout_2->addLayout(verticalLayout_5, 1, 2, 1, 1);


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
        QObject::connect(pushButtonYUV, SIGNAL(clicked()), MainWindow, SLOT(on_pushButtonYUV_clicked()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        open->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
        labelLine->setText(QString());
        labelParell->setText(QString());
        labelOpenMP->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        ButtonNoise->setText(QCoreApplication::translate("MainWindow", "\320\250\321\203\320\274", nullptr));
        ButtonGauss->setText(QCoreApplication::translate("MainWindow", "\320\244. \320\223\320\260\321\203\321\201\321\201\320\260", nullptr));
        ButtonRezk->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\262\321\213\321\210. \321\200\320\265\320\267\320\272\320\276\321\201\321\202\320\270", nullptr));
        pushButtonYUV->setText(QCoreApplication::translate("MainWindow", "YUV", nullptr));
        ButtonAquarel->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        ButtonMy->setText(QCoreApplication::translate("MainWindow", "U", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "V", nullptr));
        ButtonRepair->setText(QCoreApplication::translate("MainWindow", "\320\241\320\261\321\200\320\276\321\201", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
