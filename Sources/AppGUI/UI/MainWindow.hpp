#ifndef MainWindow_HPP
#define MainWindow_HPP

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btnRun;
    QListView *appList;
    QPushButton *btnApplicationAdd;
    QPushButton *btnUnitSelector;
    QLabel *labelUnitSelected;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(660, 460);
        MainWindow->setFixedSize(QSize(660, 460));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        btnRun = new QPushButton(centralwidget);
        btnRun->setObjectName("btnRun");
        btnRun->setGeometry(QRect(530, 310, 111, 34));
        appList = new QListView(centralwidget);
        appList->setObjectName("appList");
        appList->setGeometry(QRect(10, 20, 641, 281));
        appList->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        appList->setWordWrap(true);
        btnApplicationAdd = new QPushButton(centralwidget);
        btnApplicationAdd->setObjectName("btnApplicationAdd");
        btnApplicationAdd->setGeometry(QRect(0, 310, 121, 34));
        btnUnitSelector = new QPushButton(centralwidget);
        btnUnitSelector->setObjectName("btnUnitSelector");
        btnUnitSelector->setGeometry(QRect(130, 310, 88, 34));
        labelUnitSelected = new QLabel(centralwidget);
        labelUnitSelected->setObjectName("labelUnitSelected");
        labelUnitSelected->setGeometry(QRect(10, 350, 631, 71));
        labelUnitSelected->setWordWrap(true);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);

        performLogic(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "RunAsGPU", nullptr));
        btnRun->setText(QCoreApplication::translate("MainWindow", "Run Application", nullptr));
        btnApplicationAdd->setText(QCoreApplication::translate("MainWindow", "Add Application", nullptr));
        btnUnitSelector->setText(QCoreApplication::translate("MainWindow", "Select GPU", nullptr));
        labelUnitSelected->setText(QCoreApplication::translate("MainWindow", "Selected GPU: {{UNIT}}", nullptr));
    } // retranslateUi

    void performLogic(QMainWindow* window) const;

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MainWindow_HPP