#ifndef AppRemoval_HPP
#define AppRemoval_HPP

#include "Model/AppListModel.hpp"

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>

QT_BEGIN_NAMESPACE

class Ui_AppRemovalDialog {
public:
    QDialog *self;
    QDialogButtonBox *buttonBox;
    QLabel *labelFilter;
    QLineEdit *filterBox;
    QListView *appListView;

    void setupUi(QDialog *AppRemovalDialog) {
        if (AppRemovalDialog->objectName().isEmpty())
            AppRemovalDialog->setObjectName("AppRemovalDialog");
        AppRemovalDialog->resize(491, 377);
        self = AppRemovalDialog;
        buttonBox = new QDialogButtonBox(AppRemovalDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(140, 330, 341, 32));
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(
                QDialogButtonBox::StandardButton::Apply | QDialogButtonBox::StandardButton::Close);
        labelFilter = new QLabel(AppRemovalDialog);
        labelFilter->setObjectName("labelFilter");
        labelFilter->setGeometry(QRect(10, 285, 41, 18));
        filterBox = new QLineEdit(AppRemovalDialog);
        filterBox->setObjectName("filterBox");
        filterBox->setGeometry(QRect(60, 280, 421, 32));
        appListView = new QListView(AppRemovalDialog);
        appListView->setObjectName("appListView");
        appListView->setGeometry(QRect(10, 10, 471, 261));

        retranslateUi(AppRemovalDialog);

        QMetaObject::connectSlotsByName(AppRemovalDialog);
        performLogic();
    } // setupUi

    void retranslateUi(QDialog *AppRemovalDialog) {
        AppRemovalDialog->setWindowTitle(QCoreApplication::translate("AppRemovalDialog", "Remove Apps", nullptr));
        labelFilter->setText(QCoreApplication::translate("AppRemovalDialog", "Filter:", nullptr));
        filterBox->setPlaceholderText(QCoreApplication::translate("AppRemovalDialog", "Application Name", nullptr));
    } // retranslateUi

    void performLogic() const;

};

namespace Ui {

    extern std::vector<Application> appsForRemoval;

    class AppRemovalDialog : public Ui_AppRemovalDialog {
    };
} // namespace Ui

QT_END_NAMESPACE

#endif // AppRemoval_HPP
