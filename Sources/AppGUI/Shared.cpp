#include "Shared.hpp"

#include <QFileInfo>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace SharedCode {

    QIcon FindIcon(const QString &name) {
        QIcon icon = QIcon::fromTheme(name);
        if (!icon.isNull())
            return icon;

        QStringList iconPaths = {
                "/usr/share/icons/hicolor/48x48/apps/" + name + ".png",
                "/usr/share/icons/hicolor/128x128/apps/" + name + ".png",
                "/usr/share/icons/hicolor/scalable/apps/" + name + ".svg",
                "/usr/share/pixmaps/" + name + ".png"
        };

        for (const QString &iconPath: iconPaths) {
            if (QFileInfo::exists(iconPath))
                return QIcon(iconPath);
        }

        return QIcon::fromTheme("application-x-executable");
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
            QString executor = app.execPath.split(' ')[0];
            if (executor.isEmpty()) continue;

            if (executor.startsWith('\"'))
                executor = executor.mid(1, executor.length());

            if (executor.endsWith('\"'))
                executor = executor.left(executor.length() - 1);

            obj["name"] = app.name;
            obj["description"] = app.description;
            obj["execPath"] = executor;
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

}