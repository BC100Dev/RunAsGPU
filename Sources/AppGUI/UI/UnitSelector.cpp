#include <QStandardItemModel>

#include "UnitSelector.hpp"
#include "RunAsGPU/Shared/Runner.hpp"

#include <UnitSelectorData.hpp>

std::vector<GraphicalUnit> gpuList;

void Ui_UnitSelector::performLogic() const {
    self->setFixedSize(self->size());

    gpuList = Runner::ListGraphicalUnits();
    QStandardItemModel *model = new QStandardItemModel(unit_list);

    for (const auto &gpu : gpuList) {
        QStandardItem *item = new QStandardItem(QString::fromStdString(gpu.fullName));
        model->appendRow(item);
    }

    unit_list->setModel(model);
    unit_list->setSelectionMode(QAbstractItemView::SingleSelection);

    QObject::connect(choiceSelect, &QDialogButtonBox::accepted, self, [&]() {
        QModelIndex index = unit_list->currentIndex();

        if (index.isValid()) {
            int gpuIndex = index.row();
            UnitSelectorData::unitSelected = gpuList[gpuIndex];
        } else
            QMessageBox::warning(self, "GPU Selection", "No GPU selected.");

        self->accept();
    });
    QObject::connect(choiceSelect, &QDialogButtonBox::rejected, self, [&]() {
        self->reject();
    });
}