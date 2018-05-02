/********************************************************************************
** Form generated from reading UI file 'dblpanalysisapp.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBLPANALYSISAPP_H
#define UI_DBLPANALYSISAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dblpAnalysisAppClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *dblpAnalysisAppClass)
    {
        if (dblpAnalysisAppClass->objectName().isEmpty())
            dblpAnalysisAppClass->setObjectName(QStringLiteral("dblpAnalysisAppClass"));
        dblpAnalysisAppClass->resize(600, 400);
        centralWidget = new QWidget(dblpAnalysisAppClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        dblpAnalysisAppClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(dblpAnalysisAppClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
        dblpAnalysisAppClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(dblpAnalysisAppClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        dblpAnalysisAppClass->setStatusBar(statusBar);

        retranslateUi(dblpAnalysisAppClass);

        QMetaObject::connectSlotsByName(dblpAnalysisAppClass);
    } // setupUi

    void retranslateUi(QMainWindow *dblpAnalysisAppClass)
    {
        dblpAnalysisAppClass->setWindowTitle(QApplication::translate("dblpAnalysisAppClass", "dblpAnalysisApp", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dblpAnalysisAppClass: public Ui_dblpAnalysisAppClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBLPANALYSISAPP_H
