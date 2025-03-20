#include "MainWindow.hpp"
#include "UnitSelector.hpp"

#include <QMessageBox>

#include <RunAsGPU/Shared/GraphicalUnit.hpp>
#include <RunAsGPU/Shared/Runner.hpp>
#include <IconFinder.hpp>

#include "Model/AppListModel.hpp"
#include "Model/AppListDelegate.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <QProcess>

namespace fs = std::filesystem;

AppListModel *model;
AppListDelegate *delegate;

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

void Ui_MainWindow::performLogic() const {
    std::vector<GraphicalUnit> gpu_list = Runner::ListGraphicalUnits();
    if (gpu_list.empty()) {
        std::cerr << "GPU list empty, exiting..." << std::endl;
        std::abort();
    }

    gpuUnit = get_default_gpu(gpu_list);
    if (gpuUnit == -1)
        gpuUnit = 0;

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

    model->addItem(Application("Blender", "3D modeling software", "/usr/bin/blender",
                               FindIcon("blender")));
    model->addItem(Application("Firefox", "Mozilla Firefox", "/usr/bin/firefox",
                               FindIcon("firefox")));

    QObject::connect(appList->selectionModel(), &QItemSelectionModel::currentChanged,
                     [&](const QModelIndex &current, const QModelIndex &) {
                         if (current.isValid()) {
                             qDebug() << "Selected App:" << current.data(Qt::DisplayRole).toString();
                         }
                     });

    QObject::connect(appList, &QListView::doubleClicked, [&](const QModelIndex &index) {
        if (index.isValid()) {
            QString execPath = index.data(Qt::UserRole).toString();
            if (!execPath.isEmpty())
                QProcess::startDetached(execPath);
        }
    });

// "Add Application" button logic
    QObject::connect(btnApplicationAdd, &QPushButton::clicked, [&]() {
        // TODO: show app selection dialog
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
            QMessageBox::warning(nullptr, "Invalid application", "The selected application has no executable path.");
            return;
        }

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
    });

    // "Select GPU" button logic
    QObject::connect(btnUnitSelector, &QPushButton::clicked, [&]() {
        QDialog dialog;
        Ui_UnitSelector ui;
        ui.setupUi(&dialog);

        std::vector<GraphicalUnit> gList = Runner::ListGraphicalUnits();
        ui.gpuList = gList;

        int rc = dialog.exec();
    });
}