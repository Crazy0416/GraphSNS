/********************************************************************************
** Form generated from reading UI file 'SNSAnalysisApp.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SNSANALYSISAPP_H
#define UI_SNSANALYSISAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SNSAnalysisAppClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SNSAnalysisAppClass)
    {
        if (SNSAnalysisAppClass->objectName().isEmpty())
            SNSAnalysisAppClass->setObjectName(QStringLiteral("SNSAnalysisAppClass"));
        SNSAnalysisAppClass->resize(600, 400);
        menuBar = new QMenuBar(SNSAnalysisAppClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        SNSAnalysisAppClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SNSAnalysisAppClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        SNSAnalysisAppClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(SNSAnalysisAppClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        SNSAnalysisAppClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SNSAnalysisAppClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        SNSAnalysisAppClass->setStatusBar(statusBar);

        retranslateUi(SNSAnalysisAppClass);

        QMetaObject::connectSlotsByName(SNSAnalysisAppClass);
    } // setupUi

    void retranslateUi(QMainWindow *SNSAnalysisAppClass)
    {
        SNSAnalysisAppClass->setWindowTitle(QApplication::translate("SNSAnalysisAppClass", "SNSAnalysisApp", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SNSAnalysisAppClass: public Ui_SNSAnalysisAppClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNSANALYSISAPP_H
