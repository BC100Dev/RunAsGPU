#include "MainWindow.hpp"
#include "UnitSelector.hpp"
#include "AppSelector.hpp"

#include <QMessageBox>
#include <QProcess>
#include <QPixmap>

#include <RunAsGPU/Shared/GraphicalUnit.hpp>
#include <RunAsGPU/Shared/Runner.hpp>
#include <IconFinder.hpp>
#include <UnitSelectorData.hpp>

#include "Model/AppListModel.hpp"
#include "Model/AppListDelegate.hpp"

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

void SaveAppList(const std::vector<Application> &appList) {
    const char *home = getenv("HOME");
    if (!home) {
        std::cerr << "Failed to get home directory, exiting..." << std::endl;
        std::abort();
    }

    std::string appListPath = std::string(home) + "/.config/RunAsGPU/apps.json";
    if (!fs::exists(fs::path(appListPath).parent_path()))
        fs::create_directory(fs::path(appListPath).parent_path());

    QJsonArray jsonArr;
    for (const auto &app: appList) {
        QJsonObject obj;
        obj["name"] = app.name;
        obj["description"] = app.description;
        obj["execPath"] = app.execPath;
        obj["icon"] = app.iconStr.isEmpty() ? QJsonValue(QJsonValue::Null) : app.iconStr;
        jsonArr.append(obj);
    }

    QJsonDocument doc(jsonArr);
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/RunAsGPU/apps.json";
    QFile file(configPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "Failed to save apps.json:" << file.errorString();
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

std::vector<Application> GetAppList() {
    std::vector<Application> appList;

    QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/RunAsGPU/apps.json";
    QFile file(configPath);

    if (!file.exists()) {
        std::ofstream _file(configPath.toStdString());
        if (_file.is_open() && _file.good()) {
            _file << "[]" << std::endl;
            _file.close();
        }
        return appList;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open apps.json: " << file.errorString();
        return appList;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isArray()) {
        qDebug() << "Invalid JSON format!";
        return appList;
    }

    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue &value: jsonArray) {
        if (!value.isObject()) continue;

        QJsonObject jsonObj = value.toObject();
        Application app{jsonObj["name"].toString(), jsonObj["desc"].toString(), jsonObj["execPath"].toString(),
                        jsonObj["icon"].toString()};

        appList.push_back(app);
    }

    return appList;
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

    labelUnitSelected->setText("Selected GPU: " + QString::fromStdString(gpu_list[gpuUnit].fullName));

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

    givenAppList = GetAppList();
    for (const auto &app: givenAppList)
        model->addItem(app);

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

                SaveAppList(givenAppList);
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

                if (ui.choiceDefUnit->isChecked()) {
                    std::cout << "Default GPU selected: " << std::hex << UnitSelectorData::unitSelected.product
                              << std::endl;
                    save_default_gpu(gpu_list, gpuUnit);
                }

                labelUnitSelected->setText(
                        "Selected GPU: " + QString::fromStdString(UnitSelectorData::unitSelected.fullName));
            } else
                QMessageBox::warning(nullptr, "GPU Find Error", "The selected GPU was not found from its list.");
        }
    });
}