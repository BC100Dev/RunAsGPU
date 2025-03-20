#include "AppSelector.hpp"

#include "Model/AppListModel.hpp"
#include "Model/AppListDelegate.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include <RunAsGPU/Shared/Shared.hpp>

#include <QFileInfo>
#include <QDir>
#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

namespace fs = std::filesystem;

std::vector<Application> ListInstalledApps() {
    std::vector<Application> appList;
    std::vector<std::string> searchPaths;

    const char *xdgDirs = getenv("XDG_DATA_DIRS");
    if (xdgDirs) {
        std::istringstream ss(xdgDirs);
        std::string path;

        while (std::getline(ss, path, ':'))
            searchPaths.push_back(path + "/applications");
    } else
        searchPaths = {"/usr/share/applications", "/usr/local/share/applications"};

    const char *homeDir = getenv("HOME");
    if (homeDir)
        searchPaths.push_back(std::string(homeDir) + "/.local/share/applications");

    for (const auto &path: searchPaths) {
        if (!fs::exists(path))
            continue;

        if (!fs::is_directory(path))
            continue;

        for (const auto &entry: fs::directory_iterator(path)) {
            if (entry.path().extension() != ".desktop")
                continue;

            QString name, description, execPath, iconStr;
            std::ifstream file(entry.path());
            if (!file.is_open())
                continue;

            std::map<std::string, std::string> desktopEntries;
            std::string line;

            bool inDesktopEntry = false;
            while (std::getline(file, line)) {
                line = TrimString(line);

                if (line.empty())
                    continue;

                if (line.front() == '[' && line.back() == ']') {
                    inDesktopEntry = true;
                    continue;
                }

                if (inDesktopEntry) {
                    size_t pos = line.find('=');
                    if (pos == std::string::npos)
                        continue;

                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    desktopEntries[key] = value;
                }
            }

            if (desktopEntries.find("Name") != desktopEntries.end())
                name = QString::fromStdString(desktopEntries["Name"]);
            if (desktopEntries.find("Comment") != desktopEntries.end())
                description = QString::fromStdString(desktopEntries["Comment"]);
            if (desktopEntries.find("Exec") != desktopEntries.end())
                execPath = QString::fromStdString(desktopEntries["Exec"]);
            if (desktopEntries.find("Icon") != desktopEntries.end())
                iconStr = QString::fromStdString(desktopEntries["Icon"]);

            Application app{name, description, execPath, iconStr};
            appList.push_back(app);
        }
    }

    return appList;
}

std::vector<Application> appList,
        filteredApps;

std::vector<QString> qSelectedApps;

std::vector<Application> Ui::selectedItems;

AppListModel *modelAppList;
AppListDelegate *delegateAppList;

void Ui_AppSelector::performLogic() const {
    modelAppList = new AppListModel(listInstalledApps);
    delegateAppList = new AppListDelegate(listInstalledApps);

    self->setFixedSize(self->size());

    listInstalledApps->setModel(modelAppList);
    listInstalledApps->setItemDelegate(delegateAppList);
    listInstalledApps->setViewMode(QListView::ListMode);
    listInstalledApps->setUniformItemSizes(true);
    listInstalledApps->setSpacing(5);
    listInstalledApps->setSelectionMode(QAbstractItemView::MultiSelection);
    listInstalledApps->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listInstalledApps->setFocusPolicy(Qt::StrongFocus);

    appList = ListInstalledApps();
    std::sort(appList.begin(), appList.end(), [](const Application &a, const Application &b) {
        return a.name.compare(b.name, Qt::CaseInsensitive) < 0;
    });

    for (const auto &app: appList) {
        if (app.execPath.isEmpty())
            continue;

        modelAppList->addItem(app);
    }

    QObject::connect(listInstalledApps->selectionModel(), &QItemSelectionModel::selectionChanged, self,
                     [&](const QItemSelection &selected, const QItemSelection &deselected) {
                         for (const QModelIndex &index: selected.indexes()) {
                             QString selectedName = modelAppList->data(index, Qt::DisplayRole).toString();

                             auto it = std::find_if(qSelectedApps.begin(), qSelectedApps.end(),
                                                    [&](const QString &appName) {
                                                        return appName == selectedName;
                                                    });

                             if (it == qSelectedApps.end()) {
                                 auto appIt = std::find_if(appList.begin(), appList.end(), [&](const Application &app) {
                                     return app.name == selectedName;
                                 });

                                 if (appIt != appList.end())
                                     qSelectedApps.push_back(selectedName);
                             }
                         }

                         for (const QModelIndex &index: deselected.indexes()) {
                             QString deselectedName = modelAppList->data(index, Qt::DisplayRole).toString();

                             qSelectedApps.erase(std::remove_if(qSelectedApps.begin(), qSelectedApps.end(),
                                                                [&](const QString &appName) {
                                                                    return appName == deselectedName;
                                                                }), qSelectedApps.end());
                         }
                     });

    QObject::connect(filterBox, &QLineEdit::textChanged, self, [&](const QString &text) {
        QItemSelectionModel *selectionModel = listInstalledApps->selectionModel();

        filteredApps.clear();
        for (const auto &app: appList) {
            if (app.name.contains(text, Qt::CaseInsensitive))
                filteredApps.push_back(app);
        }

        modelAppList->clear();
        for (const auto &app: filteredApps)
            modelAppList->addItem(app);

        // restore selections
        for (int i = 0; i < modelAppList->rowCount(); i++) {
            QModelIndex index = modelAppList->index(i, 0);
            QString appName = modelAppList->data(index, Qt::DisplayRole).toString();
            if (std::find(qSelectedApps.begin(), qSelectedApps.end(), appName) != qSelectedApps.end())
                selectionModel->select(index, QItemSelectionModel::Select);
        }
    });

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, self, [&]() {
        Ui::selectedItems.clear();

        for (const QString &index: qSelectedApps) {
            auto it = std::find_if(appList.begin(), appList.end(), [&](const Application &app) {
                return app.name == index;
            });

            if (it != appList.end()) {
                Ui::selectedItems.push_back(*it);
            }
        }

        listInstalledApps->selectionModel()->clearSelection();

        self->accept();
    });

    QObject::connect(buttonBox, &QDialogButtonBox::rejected, self, [&]() {
        self->reject();
    });
}