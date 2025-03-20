#ifndef APPSELECTOR_HPP
#define APPSELECTOR_HPP

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include "Model/AppListModel.hpp"

QT_BEGIN_NAMESPACE

class Ui_AppSelector
{
public:
    QDialog *self;
    QDialogButtonBox *buttonBox;
    QListView *listInstalledApps;
    QLabel *label0;
    QLineEdit *filterBox;
    QLabel *label1;

    void setupUi(QDialog *AppSelector)
    {
        if (AppSelector->objectName().isEmpty())
            AppSelector->setObjectName("AppSelector");
        AppSelector->resize(590, 433);
        self = AppSelector;
        buttonBox = new QDialogButtonBox(AppSelector);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(240, 390, 341, 32));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);
        listInstalledApps = new QListView(AppSelector);
        listInstalledApps->setObjectName("listInstalledApps");
        listInstalledApps->setGeometry(QRect(10, 40, 571, 281));
        label0 = new QLabel(AppSelector);
        label0->setObjectName("label0");
        label0->setGeometry(QRect(10, 10, 151, 18));
        filterBox = new QLineEdit(AppSelector);
        filterBox->setObjectName("filterBox");
        filterBox->setGeometry(QRect(50, 330, 531, 32));
        label1 = new QLabel(AppSelector);
        label1->setObjectName("label1");
        label1->setGeometry(QRect(10, 335, 58, 18));

        retranslateUi(AppSelector);

        QMetaObject::connectSlotsByName(AppSelector);

        performLogic();
    } // setupUi

    void retranslateUi(QDialog *AppSelector)
    {
        AppSelector->setWindowTitle(QCoreApplication::translate("AppSelector", "Select Application", nullptr));
        label0->setText(QCoreApplication::translate("AppSelector", "Installed applications:", nullptr));
        filterBox->setPlaceholderText(QCoreApplication::translate("AppSelector", "Application Name", nullptr));
        label1->setText(QCoreApplication::translate("AppSelector", "Filter:", nullptr));
    } // retranslateUi

    void performLogic() const;

};

namespace Ui {
    extern std::vector<Application> selectedItems;

    class AppSelector: public Ui_AppSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // APPSELECTOR_HPP
