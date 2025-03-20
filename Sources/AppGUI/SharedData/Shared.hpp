#ifndef RUNASGPU_SHARED_HPP
#define RUNASGPU_SHARED_HPP

#include <QIcon>
#include <QString>

#include "../UI/Model/AppListModel.hpp"

namespace SharedCode {

    QIcon FindIcon(const QString &iconName);

    std::vector<Application> GetAppList();

    void SaveAppList(const std::vector<Application> &appList);

}

#endif //RUNASGPU_SHARED_HPP
