#include "AppListDelegate.hpp"

AppListDelegate::AppListDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void AppListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();

    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
    else if (option.state & QStyle::State_MouseOver)
        painter->fillRect(option.rect, option.palette.light());
    else
        painter->fillRect(option.rect, option.palette.base());

    QRect rect = option.rect;
    QVariant var = index.data(Qt::DecorationRole);
    QIcon icon;

    if (var.metaType() == QMetaType::fromType<QIcon>())
        icon = var.value<QIcon>();

    QString name = index.data(Qt::DisplayRole).toString();

    QRect iconRect = QRect(rect.left() + 5, rect.top() + 5, 32, 32);
    icon.paint(painter, iconRect);

    QRect textRect = QRect(iconRect.right() + 12, rect.top(), rect.width() - 50, rect.height());
    painter->drawText(textRect, Qt::AlignVCenter, name);

    painter->restore();
}

QSize AppListDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const {
    return QSize(150, 40);
}