#include "AppRemoval.hpp"
#include "Model/AppListDelegate.hpp"

#include <Shared.hpp>
#include <iostream>

std::vector<Application> Ui::appsForRemoval;

AppListModel *modelAppList0;
AppListDelegate *delegateAppList0;

std::vector<QString> rSelectedApps;
std::vector<Application> rFilteredApps, rApps;

void Ui_AppRemovalDialog::performLogic() const {
    self->setFixedSize(self->size());

    modelAppList0 = new AppListModel(appListView);
    delegateAppList0 = new AppListDelegate(appListView);

    self->setFixedSize(self->size());

    appListView->setModel(modelAppList0);
    appListView->setItemDelegate(delegateAppList0);
    appListView->setViewMode(QListView::ListMode);
    appListView->setUniformItemSizes(true);
    appListView->setSpacing(5);
    appListView->setSelectionMode(QAbstractItemView::MultiSelection);
    appListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    appListView->setFocusPolicy(Qt::StrongFocus);

    rApps = SharedCode::GetAppList();
    for (const auto &app: rApps)
        modelAppList0->addItem(app);

    QObject::connect(filterBox, &QLineEdit::textChanged, self, [&](const QString &text) {
        QItemSelectionModel *selectionModel = appListView->selectionModel();

        rFilteredApps.clear();
        for (const auto &app: rApps) {
            if (app.name.contains(text, Qt::CaseInsensitive))
                rFilteredApps.push_back(app);
        }

        modelAppList0->clear();
        for (const auto &app: rFilteredApps)
            modelAppList0->addItem(app);

        // restore selections
        for (int i = 0; i < modelAppList0->rowCount(); i++) {
            QModelIndex index = modelAppList0->index(i, 0);
            QString appName = modelAppList0->data(index, Qt::DisplayRole).toString();
            if (std::find(rSelectedApps.begin(), rSelectedApps.end(), appName) != rSelectedApps.end())
                selectionModel->select(index, QItemSelectionModel::Select);
        }
    });

    QObject::connect(appListView->selectionModel(), &QItemSelectionModel::selectionChanged, self,
                     [&](const QItemSelection &selected, const QItemSelection &deselected) {
                         for (const QModelIndex &index: selected.indexes()) {
                             QString selectedName = modelAppList0->data(index, Qt::DisplayRole).toString();

                             auto it = std::find_if(rSelectedApps.begin(), rSelectedApps.end(),
                                                    [&](const QString &appName) {
                                                        return appName == selectedName;
                                                    });

                             if (it == rSelectedApps.end()) {
                                 auto appIt = std::find_if(rApps.begin(), rApps.end(), [&](const Application &app) {
                                     return app.name == selectedName;
                                 });

                                 if (appIt != rApps.end())
                                     rSelectedApps.push_back(selectedName);
                             }
                         }

                         for (const QModelIndex &index: deselected.indexes()) {
                             QString deselectedName = modelAppList0->data(index, Qt::DisplayRole).toString();

                             rSelectedApps.erase(std::remove_if(rSelectedApps.begin(), rSelectedApps.end(),
                                                                [&](const QString &appName) {
                                                                    return appName == deselectedName;
                                                                }), rSelectedApps.end());
                         }
                     });

    QObject::connect(buttonBox, &QDialogButtonBox::clicked, self, [&](QAbstractButton *button) {
        if (buttonBox->standardButton(button) != QDialogButtonBox::Apply)
            return;

        Ui::appsForRemoval.clear();

        for (const QString &index: rSelectedApps) {
            auto it = std::find_if(rApps.begin(), rApps.end(), [&](const Application &app) {
                return app.name == index;
            });

            if (it != rApps.end()) {
                Ui::appsForRemoval.push_back(*it);
            }
        }

        appListView->selectionModel()->clearSelection();
        self->accept();
    });

    QObject::connect(buttonBox, &QDialogButtonBox::rejected, self, [&]() {
        self->reject();
    });
}