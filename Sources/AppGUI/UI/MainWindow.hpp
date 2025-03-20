#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

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

class Ui_MainWindow {
public:
    QWidget *centralwidget;
    QPushButton *btnRun;
    QListView *appList;
    QPushButton *btnApplicationAdd;
    QPushButton *btnUnitSelector;
    QLabel *labelUnitSelected;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow) {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(586, 421);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(586, 421));
        MainWindow->setMaximumSize(QSize(586, 421));
        QIcon icon;
        icon.addFile(QString::fromUtf8("RunAsGPU_Icon.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        btnRun = new QPushButton(centralwidget);
        btnRun->setObjectName("btnRun");
        btnRun->setGeometry(QRect(467, 310, 111, 34));
        appList = new QListView(centralwidget);
        appList->setObjectName("appList");
        appList->setGeometry(QRect(0, 20, 581, 281));
        appList->setStyleSheet("QListView::item:selected { background-color: #0078d7; }");
        appList->setFocusPolicy(Qt::StrongFocus);
        appList->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        btnApplicationAdd = new QPushButton(centralwidget);
        btnApplicationAdd->setObjectName("btnApplicationAdd");
        btnApplicationAdd->setGeometry(QRect(0, 310, 121, 34));
        btnUnitSelector = new QPushButton(centralwidget);
        btnUnitSelector->setObjectName("btnUnitSelector");
        btnUnitSelector->setGeometry(QRect(130, 310, 88, 34));
        labelUnitSelected = new QLabel(centralwidget);
        labelUnitSelected->setObjectName("labelUnitSelected");
        labelUnitSelected->setGeometry(QRect(0, 350, 571, 31));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);

        performLogic();
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow) const {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "RunAsGPU", nullptr));
        btnRun->setText(QCoreApplication::translate("MainWindow", "Run Application", nullptr));
        btnApplicationAdd->setText(QCoreApplication::translate("MainWindow", "Add Application", nullptr));
        btnUnitSelector->setText(QCoreApplication::translate("MainWindow", "Select GPU", nullptr));
        labelUnitSelected->setText(QCoreApplication::translate("MainWindow", "Selected GPU: {{UNIT}}", nullptr));
    } // retranslateUi

    void performLogic() const;

};

namespace Ui {
    class MainWindow : public Ui_MainWindow {
    };
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOW_HPP
