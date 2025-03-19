#ifndef UNITSELECTORFZARYN_H
#define UNITSELECTORFZARYN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QCheckBox>
#include <QMessageBox>

#include "RunAsGPU/Shared/GraphicalUnit.hpp"

QT_BEGIN_NAMESPACE

class Ui_UnitSelector {
public:
    QDialogButtonBox *choiceSelect{};
    QListView *unit_list{};
    QCheckBox *choiceDefUnit;

    std::vector<GraphicalUnit> gpuList;

    void setupUi(QDialog *UnitSelector) {
        if (UnitSelector->objectName().isEmpty())
            UnitSelector->setObjectName("UnitSelector");
        UnitSelector->resize(400, 300);
        choiceSelect = new QDialogButtonBox(UnitSelector);
        choiceSelect->setObjectName("choiceSelect");
        choiceSelect->setGeometry(QRect(50, 250, 341, 32));
        choiceSelect->setOrientation(Qt::Orientation::Horizontal);
        choiceSelect->setStandardButtons(
                QDialogButtonBox::StandardButton::Cancel | QDialogButtonBox::StandardButton::Ok);
        unit_list = new QListView(UnitSelector);
        unit_list->setObjectName("unit_list");
        unit_list->setGeometry(QRect(0, 10, 401, 221));
        choiceDefUnit = new QCheckBox(UnitSelector);
        choiceDefUnit->setObjectName("choiceDefUnit");
        choiceDefUnit->setGeometry(QRect(0, 240, 161, 22));

        retranslateUi(UnitSelector);
        QObject::connect(choiceSelect, &QDialogButtonBox::accepted, UnitSelector, [&]() {
            QModelIndex index = unit_list->currentIndex();

            if (index.isValid()) {
                int gpuIndex = index.row();
                GraphicalUnit unit = gpuList.at(gpuIndex);
            } else
                QMessageBox::warning(UnitSelector, "GPU Selection", "No valid GPU selected.");

            onItemSelect(true);
            UnitSelector->accept();
        });
        QObject::connect(choiceSelect, &QDialogButtonBox::rejected, UnitSelector, [&]() {
            onItemSelect(false);
            UnitSelector->reject();
        });

        QMetaObject::connectSlotsByName(UnitSelector);

        performLogic();
    } // setupUi

    void retranslateUi(QDialog *UnitSelector) {
        UnitSelector->setWindowTitle(QCoreApplication::translate("UnitSelector", "Select GPU", nullptr));
        choiceDefUnit->setText(QCoreApplication::translate("UnitSelector", "Set as default GPU", nullptr));
    } // retranslateUi

    void performLogic() const;

    void onItemSelect(bool accept) const;

};

namespace Ui {
    class UnitSelector : public Ui_UnitSelector {
    };
} // namespace Ui

QT_END_NAMESPACE

#endif // UNITSELECTORFZARYN_H
