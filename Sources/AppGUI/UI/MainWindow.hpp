/********************************************************************************
** Form generated from reading UI file 'MainWindowaRdqRS.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWARDQRS_H
#define MAINWINDOWARDQRS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow {
public:
    QMainWindow *self;
    QWidget *centralwidget;
    QPushButton *btnRun;
    QListView *appList;
    QPushButton *btnApplicationAdd;
    QPushButton *btnUnitSelector;
    QLabel *labelUnitSelected;
    QLabel *labelFilter;
    QLineEdit *filterBox;
    QPushButton *btnRemove;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow) {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(660, 505);
        self = MainWindow;
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(660, 460));
        MainWindow->setMaximumSize(QSize(15000, 15000));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        btnRun = new QPushButton(centralwidget);
        btnRun->setObjectName("btnRun");
        btnRun->setGeometry(QRect(540, 350, 111, 34));
        appList = new QListView(centralwidget);
        appList->setObjectName("appList");
        appList->setGeometry(QRect(10, 20, 641, 281));
        appList->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        appList->setWordWrap(true);
        btnApplicationAdd = new QPushButton(centralwidget);
        btnApplicationAdd->setObjectName("btnApplicationAdd");
        btnApplicationAdd->setGeometry(QRect(10, 350, 121, 34));
        btnUnitSelector = new QPushButton(centralwidget);
        btnUnitSelector->setObjectName("btnUnitSelector");
        btnUnitSelector->setGeometry(QRect(140, 350, 88, 34));
        labelUnitSelected = new QLabel(centralwidget);
        labelUnitSelected->setObjectName("labelUnitSelected");
        labelUnitSelected->setGeometry(QRect(10, 400, 511, 61));
        labelUnitSelected->setWordWrap(true);
        labelFilter = new QLabel(centralwidget);
        labelFilter->setObjectName("labelFilter");
        labelFilter->setGeometry(QRect(10, 315, 41, 18));
        filterBox = new QLineEdit(centralwidget);
        filterBox->setObjectName("filterBox");
        filterBox->setGeometry(QRect(60, 310, 591, 32));
        btnRemove = new QPushButton(centralwidget);
        btnRemove->setObjectName("btnRemove");
        btnRemove->setEnabled(true);
        btnRemove->setGeometry(QRect(540, 390, 111, 34));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
        performLogic();
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow) {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "RunAsGPU", nullptr));
        btnRun->setText(QCoreApplication::translate("MainWindow", "Run Application", nullptr));
        btnApplicationAdd->setText(QCoreApplication::translate("MainWindow", "Add Application", nullptr));
        btnUnitSelector->setText(QCoreApplication::translate("MainWindow", "Select GPU", nullptr));
        labelUnitSelected->setText(
                QCoreApplication::translate("MainWindow", "Selected GPU: <b><i>{{UNIT}}</i></b>", nullptr));
        labelFilter->setText(QCoreApplication::translate("MainWindow", "Filter:", nullptr));
        filterBox->setPlaceholderText(QCoreApplication::translate("MainWindow", "Application Name", nullptr));
        btnRemove->setText(QCoreApplication::translate("MainWindow", "Remove App", nullptr));
    } // retranslateUi

    void performLogic() const;

};

namespace Ui {
    class MainWindow : public Ui_MainWindow {
    };
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWARDQRS_H
