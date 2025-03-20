#ifndef AppSelector_HPP
#define AppSelector_HPP

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>

QT_BEGIN_NAMESPACE

class Ui_AppSelector
{
public:
    QDialogButtonBox *buttonBox;
    QListView *listInstalledApps;
    QLabel *label0;

    void setupUi(QDialog *AppSelector)
    {
        if (AppSelector->objectName().isEmpty())
            AppSelector->setObjectName("AppSelector");
        AppSelector->resize(590, 385);
        buttonBox = new QDialogButtonBox(AppSelector);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(230, 340, 341, 32));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);
        listInstalledApps = new QListView(AppSelector);
        listInstalledApps->setObjectName("listInstalledApps");
        listInstalledApps->setGeometry(QRect(10, 40, 571, 281));
        label0 = new QLabel(AppSelector);
        label0->setObjectName("label0");
        label0->setGeometry(QRect(10, 10, 151, 18));

        retranslateUi(AppSelector);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, AppSelector, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, AppSelector, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(AppSelector);
    } // setupUi

    void retranslateUi(QDialog *AppSelector)
    {
        AppSelector->setWindowTitle(QCoreApplication::translate("AppSelector", "Select Application", nullptr));
        label0->setText(QCoreApplication::translate("AppSelector", "Installed applications:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AppSelector: public Ui_AppSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // AppSelector_HPP
