/********************************************************************************
** Form generated from reading UI file 'ganttwidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GANTTWIDGET_H
#define UI_GANTTWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QWidget>
#include "mygantt_graphicsview.h"
#include "mygantt_treeview.h"

QT_BEGIN_NAMESPACE

class Ui_GanttWidget
{
public:
    QGridLayout *gridLayout;
    QSplitter *splitter;
    GanttTreeView *treeView;
    GanttGraphicsView *ganttView;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBox_mode;

    void setupUi(QWidget *GanttWidget)
    {
        if (GanttWidget->objectName().isEmpty())
            GanttWidget->setObjectName(QString::fromUtf8("GanttWidget"));
        GanttWidget->resize(400, 300);
        gridLayout = new QGridLayout(GanttWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(GanttWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        treeView = new GanttTreeView(splitter);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        splitter->addWidget(treeView);
        ganttView = new GanttGraphicsView(splitter);
        ganttView->setObjectName(QString::fromUtf8("ganttView"));
        splitter->addWidget(ganttView);

        gridLayout->addWidget(splitter, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        comboBox_mode = new QComboBox(GanttWidget);
        comboBox_mode->setObjectName(QString::fromUtf8("comboBox_mode"));

        gridLayout->addWidget(comboBox_mode, 1, 1, 1, 1);


        retranslateUi(GanttWidget);

        QMetaObject::connectSlotsByName(GanttWidget);
    } // setupUi

    void retranslateUi(QWidget *GanttWidget)
    {
        GanttWidget->setWindowTitle(QApplication::translate("GanttWidget", "GanttWidget", 0, QApplication::UnicodeUTF8));
        comboBox_mode->clear();
        comboBox_mode->insertItems(0, QStringList()
         << QApplication::translate("GanttWidget", "Months", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("GanttWidget", "Days", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("GanttWidget", "Hours", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("GanttWidget", "Minutes", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("GanttWidget", "Seconds", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class GanttWidget: public Ui_GanttWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GANTTWIDGET_H
