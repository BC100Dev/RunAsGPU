#include "MainWindow.hpp"
#include "UnitSelector.hpp"

#include <QMessageBox>
#include <QStringListModel>

#include <RunAsGPU/Shared/GraphicalUnit.hpp>
#include <RunAsGPU/Shared/Runner.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

static QStringListModel appListModel;
static QStringList appEntries;

int get_default_gpu(const std::vector<GraphicalUnit> &gpu_list) {
    const char *home_dir = getenv("HOME");
    if (!home_dir)
        return -1;

    std::string file_path = std::string(home_dir) + "/.config/RunAsGPU/gpu_identifier";
    if (!fs::exists(file_path))
        return 0;

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

void Ui_MainWindow::performLogic() const {
    std::vector<GraphicalUnit> gpu_list = Runner::ListGraphicalUnits();
    if (gpu_list.empty()) {
        std::cerr << "GPU list empty, exiting..." << std::endl;
        std::abort();
        return;
    }

    int gpuUnit = get_default_gpu(gpu_list);

    appListModel.setStringList(appEntries);
    appList->setModel(&appListModel);

// "Add Application" button logic
    QObject::connect(btnApplicationAdd, &QPushButton::clicked, [&]() {
        QString appEntry = "New Application Entry";  // Placeholder for actual selection logic
        appEntries.append(appEntry);  // Add entry to QStringList
        appListModel.setStringList(appEntries);  // Update QListView
    });

    // "Run Application" button logic
    QObject::connect(btnRun, &QPushButton::clicked, [&]() {
        if (appEntries.isEmpty()) {
            QMessageBox::warning(nullptr, "Run Application", "No application selected.");
        } else {
            QMessageBox::information(nullptr, "Run Application", "Running: " + appEntries.first());
            // TODO: Implement actual GPU switching logic
        }
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