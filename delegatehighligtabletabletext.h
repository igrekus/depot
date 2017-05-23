#ifndef DELEGATEHIGHLIGTABLETABLETEXT_H
#define DELEGATEHIGHLIGTABLETABLETEXT_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QSortFilterProxyModel>

#include <constants.h>

class DelegateHighligtableTableText : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DelegateHighligtableTableText(QObject *parent = nullptr):
        QStyledItemDelegate(parent)
    {Q_UNUSED(parent)}
    ~DelegateHighligtableTableText(){}

    void drawHighlight(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QString searchStr = qobject_cast<const QSortFilterProxyModel *>(index.model())->filterRegExp().pattern();
        QString text = index.data(Qt::DisplayRole).toString();
        if (text.contains(searchStr, Qt::CaseInsensitive)) {
            qint32 pos = text.indexOf(searchStr, 0, Qt::CaseInsensitive);
            qint32 len = searchStr.size();
            QString strToHighlight = text.mid(pos, len);

            QFontMetrics fm = painter->fontMetrics();

            QRect textRect = fm.boundingRect(strToHighlight);
            QRect shiftRect = fm.boundingRect(text.left(pos));

            shiftRect.moveTo(option.rect.topLeft());
            textRect.moveTo(shiftRect.topRight());
            painter->fillRect(textRect.adjusted(+2, 0, +5, 0), QColor(Constants::ColorSearchHighligh));
        }
    }
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        painter->save();
        drawHighlight(painter, option, index);
        painter->restore();
        QStyledItemDelegate::paint(painter, option, index);
    }
};

#endif // DELEGATEHIGHLIGTABLETABLETEXT_H
