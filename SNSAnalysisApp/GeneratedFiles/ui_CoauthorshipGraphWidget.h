/********************************************************************************
** Form generated from reading UI file 'CoauthorshipGraphWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COAUTHORSHIPGRAPHWIDGET_H
#define UI_COAUTHORSHIPGRAPHWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CoauthorshipGraphWidgetClass
{
public:

    void setupUi(QWidget *CoauthorshipGraphWidgetClass)
    {
        if (CoauthorshipGraphWidgetClass->objectName().isEmpty())
            CoauthorshipGraphWidgetClass->setObjectName(QStringLiteral("CoauthorshipGraphWidgetClass"));
        CoauthorshipGraphWidgetClass->resize(600, 400);

        retranslateUi(CoauthorshipGraphWidgetClass);

        QMetaObject::connectSlotsByName(CoauthorshipGraphWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *CoauthorshipGraphWidgetClass)
    {
        CoauthorshipGraphWidgetClass->setWindowTitle(QApplication::translate("CoauthorshipGraphWidgetClass", "CoauthorshipGraphWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CoauthorshipGraphWidgetClass: public Ui_CoauthorshipGraphWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COAUTHORSHIPGRAPHWIDGET_H
