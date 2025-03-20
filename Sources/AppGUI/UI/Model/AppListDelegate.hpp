#ifndef RUNASGPU_APPLISTDELEGATE_HPP
#define RUNASGPU_APPLISTDELEGATE_HPP

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>

class AppListDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit AppListDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif //RUNASGPU_APPLISTDELEGATE_HPP
