#include <IconFinder.hpp>
#include <QFileInfo>

QIcon FindIcon(const QString& name) {
    QIcon icon = QIcon::fromTheme(name);
    if (!icon.isNull())
        return icon;

    QStringList iconPaths = {
            "/usr/share/icons/hicolor/48x48/apps/" + name + ".png",
            "/usr/share/icons/hicolor/128x128/apps/" + name + ".png",
            "/usr/share/icons/hicolor/scalable/apps/" + name + ".svg",
            "/usr/share/pixmaps/" + name + ".png"
    };

    for (const QString& iconPath : iconPaths) {
        if (QFileInfo::exists(iconPath))
            return QIcon(iconPath);
    }

    return QIcon::fromTheme("application-x-executable");
}