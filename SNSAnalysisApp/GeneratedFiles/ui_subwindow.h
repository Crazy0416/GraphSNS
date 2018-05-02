/********************************************************************************
** Form generated from reading UI file 'subwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SUBWINDOW_H
#define UI_SUBWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_subwindow
{
public:

    void setupUi(QWidget *subwindow)
    {
        if (subwindow->objectName().isEmpty())
            subwindow->setObjectName(QStringLiteral("subwindow"));
        subwindow->resize(400, 300);

        retranslateUi(subwindow);

        QMetaObject::connectSlotsByName(subwindow);
    } // setupUi

    void retranslateUi(QWidget *subwindow)
    {
        subwindow->setWindowTitle(QApplication::translate("subwindow", "subwindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class subwindow: public Ui_subwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SUBWINDOW_H
