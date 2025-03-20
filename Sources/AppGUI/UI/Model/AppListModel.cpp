#include "AppListModel.hpp"

AppListModel::AppListModel(QObject *parent) : QAbstractListModel() {}

int AppListModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;

    return apps.size();
}

QVariant AppListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= apps.size())
        return QVariant();

    const Application &app = apps[index.row()];

    switch (role) {
        case Qt::DisplayRole:
            return app.name;
        case Qt::DecorationRole:
            return app.icon;
        case Qt::ToolTipRole:
            return app.description;
        case Qt::UserRole:
            return app.execPath;
        default:
            return QVariant();
    }
}

Qt::ItemFlags AppListModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void AppListModel::addItem(const Application &app) {
    beginInsertRows(QModelIndex(), apps.size(), apps.size());
    apps.append(app);
    endInsertRows();
}

void AppListModel::clear() {
    beginResetModel();
    apps.clear();
    endResetModel();
}