#include "MainWindow.hpp"
#include "UnitSelector.hpp"
#include "AppSelector.hpp"
#include "AppRemoval.hpp"

#include <QMessageBox>
#include <QProcess>
#include <QPixmap>

#include <RunAsGPU/Shared/GraphicalUnit.hpp>
#include <RunAsGPU/Shared/Runner.hpp>
#include <UnitSelectorData.hpp>

#include "Model/AppListModel.hpp"
#include "Model/AppListDelegate.hpp"

#include <Shared.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QFile>

namespace fs = std::filesystem;

void run_app(int gpuUnit, const QString &execPath) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("DRI_PRIME", QString::number(gpuUnit));

    auto *process = new QProcess();
    process->setProcessEnvironment(env);
    process->setProgram(execPath);
    process->setArguments(QStringList());

    qDebug() << "Running: " << execPath;
    process->start();
    if (!process->waitForStarted()) {
        QMessageBox::warning(nullptr, "Execution Failed", "Failed to start application: " + execPath);
        delete process;
        return;
    }

    process->disconnect();
    process->setParent(nullptr);
}

void save_default_gpu(const std::vector<GraphicalUnit> &gpu_list, int gpu_unit) {
    const char *home_dir = getenv("HOME");
    if (!home_dir)
        return;

    std::string dir_path = std::string(home_dir) + "/.config/RunAsGPU";
    if (!fs::exists(dir_path)) {
        if (!fs::create_directory(dir_path)) {
            std::cerr << "Failed to create config directory, exiting application" << std::endl;
            std::abort();
        }
    }

    std::string file_path = std::string(home_dir) + "/.config/RunAsGPU/gpu_identifier";
    std::ofstream file(file_path);
    if (file.is_open()) {
        file << "GPU_VENDOR_ID=" << std::hex << gpu_list[gpu_unit].vendor << std::endl;
        file << "GPU_DEVICE_ID=" << std::hex << gpu_list[gpu_unit].product << std::endl;
        file.close();
    } else
        std::cerr << "Failed to open file for writing: " << file_path << std::endl;
}

int get_default_gpu(const std::vector<GraphicalUnit> &gpu_list) {
    const char *home_dir = getenv("HOME");
    if (!home_dir)
        return -1;

    std::string file_path = std::string(home_dir) + "/.config/RunAsGPU/gpu_identifier";
    if (!fs::exists(file_path)) {
        save_default_gpu(gpu_list, 0);
        return 0;
    }

    std::string VID, DID;
    std::ifstream file(file_path);
    if (file.is_open()) {
        std::string line;

        while (std::getline(file, line)) {
            if (line.find("GPU_VENDOR_ID=") != std::string::npos)
                VID = line.substr(line.find('=') + 1);
            if (line.find("GPU_DEVICE_ID=") != std::string::npos)
                DID = line.substr(line.find('=') + 1);
        }

        file.close();

        if (VID.empty() || DID.empty())
            return -1;
    }

    for (int i = 0; i < gpu_list.size(); i++) {
        if (gpu_list[i].vendor == std::stoi(VID, nullptr, 16) &&
            gpu_list[i].product == std::stoi(DID, nullptr, 16))
            return i;
    }

    return 0;
}

int gpuUnit;
std::vector<GraphicalUnit> gpu_list;

AppListModel *model;
AppListDelegate *delegate;

std::vector<Application> givenAppList;

void Ui_MainWindow::performLogic() const {
    gpu_list = Runner::ListGraphicalUnits();
    if (gpu_list.empty()) {
        std::cerr << "GPU list empty, exiting..." << std::endl;
        std::abort();
    }

    gpuUnit = get_default_gpu(gpu_list);
    if (gpuUnit == -1)
        gpuUnit = 0;

    labelUnitSelected->setText(
            "Selected GPU: <b><i>" + QString::fromStdString(gpu_list[gpuUnit].fullName) + "</i></b>");

    self->setWindowIcon(QIcon(":/AppIcon.png"));
    self->setFixedSize(self->size());

    model = new AppListModel(appList);
    delegate = new AppListDelegate(appList);

    appList->setModel(model);
    appList->setItemDelegate(delegate);
    appList->setViewMode(QListView::ListMode);
    appList->setUniformItemSizes(true);
    appList->setSpacing(5);
    appList->setSelectionMode(QAbstractItemView::SingleSelection);
    appList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    appList->setFocusPolicy(Qt::StrongFocus);

    givenAppList = SharedCode::GetAppList();
    if (givenAppList.empty())
        btnRemove->setEnabled(false);
    else {
        for (const auto &app: givenAppList)
            model->addItem(app);
    }

    QObject::connect(appList->selectionModel(), &QItemSelectionModel::currentChanged,
                     [&](const QModelIndex &current, const QModelIndex &) {
                         if (current.isValid()) {
                             qDebug() << "Selected App:" << current.data(Qt::DisplayRole).toString();
                         }
                     });

    QObject::connect(appList, &QListView::doubleClicked, [&](const QModelIndex &index) {
        if (index.isValid()) {
            QString execPath = index.data(Qt::UserRole).toString();
            if (execPath.isEmpty()) {
                qDebug() << "Invalid application (no executable path given)";
                QMessageBox::warning(nullptr, "Invalid application",
                                     "The selected application has no executable path.");
                return;
            }

            run_app(gpuUnit, execPath);
        }
    });

    // "Add Application" button logic
    QObject::connect(btnApplicationAdd, &QPushButton::clicked, [&]() {
        QDialog dialog;
        Ui_AppSelector ui;
        ui.setupUi(&dialog);

        int rc = dialog.exec();
        if (rc == QDialog::Accepted) {
            for (const auto &app: Ui::selectedItems) {
                if (!givenAppList.empty()) {
                    auto it = std::find_if(givenAppList.begin(), givenAppList.end(), [&](const Application &a) {
                        return a.name == app.name;
                    });

                    if (it != givenAppList.end())
                        continue;
                }

                model->addItem(app);
                givenAppList.push_back(app);
                btnRemove->setEnabled(true);

                SharedCode::SaveAppList(givenAppList);
            }

            Ui::selectedItems.clear();
        }
    });

    // "Run Application" button logic
    QObject::connect(btnRun, &QPushButton::clicked, [&]() {
        QModelIndex index = appList->currentIndex();
        if (!index.isValid()) {
            qDebug() << "No application selected";
            QMessageBox::warning(nullptr, "No Application selected", "Please select an application to run.");
            return;
        }

        QString execPath = index.data(Qt::UserRole).toString();
        if (execPath.isEmpty()) {
            qDebug() << "Invalid application (no executable path given)";
            QMessageBox::warning(nullptr, "Invalid application", "The selected application has no executable path.");
            return;
        }

        run_app(gpuUnit, execPath);
    });

    QObject::connect(btnRemove, &QPushButton::clicked, [&]() {
        QDialog dialog;
        Ui_AppRemovalDialog ui;
        ui.setupUi(&dialog);

        int rc = dialog.exec();
        if (rc == QDialog::Accepted) {
            for (const auto &app: Ui::appsForRemoval) {
                auto it = std::find_if(givenAppList.begin(), givenAppList.end(), [&](const Application &a) {
                    return a.name == app.name;
                });

                if (it != givenAppList.end()) {
                    givenAppList.erase(it);
                    model->removeItem(app.name);
                }
            }

            SharedCode::SaveAppList(givenAppList);

            if (givenAppList.empty())
                btnRemove->setEnabled(false);
        }
    });

    // "Select GPU" button logic
    QObject::connect(btnUnitSelector, &QPushButton::clicked, [&]() {
        QDialog dialog;
        Ui_UnitSelector ui;
        ui.setupUi(&dialog);

        int rc = dialog.exec();
        if (rc == QDialog::Accepted) {
            auto it = std::find_if(gpu_list.begin(), gpu_list.end(), [&](const GraphicalUnit &gpu) {
                return gpu.vendor == UnitSelectorData::unitSelected.vendor &&
                       gpu.product == UnitSelectorData::unitSelected.product;
            });

            if (it != gpu_list.end()) {
                gpuUnit = std::distance(gpu_list.begin(), it);

                if (ui.choiceDefUnit->isChecked())
                    save_default_gpu(gpu_list, gpuUnit);

                labelUnitSelected->setText(
                        "Selected GPU: <b><i>" + QString::fromStdString(UnitSelectorData::unitSelected.fullName) +
                        "</i></b>");
            } else
                QMessageBox::warning(nullptr, "GPU Find Error", "The selected GPU was not found from its list.");
        }
    });
}