#ifndef RUNASGPU_APPLISTMODEL_HPP
#define RUNASGPU_APPLISTMODEL_HPP

#include <QString>
#include <QIcon>
#include <QAbstractListModel>

struct Application {
    QString name;
    QString description;
    QString execPath;
    QString iconStr;

    Application(const QString &n, const QString &d, const QString &e, const QString &i)
            : name(n), description(d), execPath(e), iconStr(i) {}
};

class AppListModel : public QAbstractListModel {
Q_OBJECT

public:
    explicit AppListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void addItem(const Application &app);
    void removeItem(const QString &appName);
    void clear();

private:
    QVector<Application> apps;
};

#endif //RUNASGPU_APPLISTMODEL_HPP
