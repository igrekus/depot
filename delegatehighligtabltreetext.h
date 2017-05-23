#ifndef DELEGATEHIGHLIGTABLTREETEXT_H
#define DELEGATEHIGHLIGTABLTREETEXT_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QSortFilterProxyModel>

#include <constants.h>
#include <delegatehighligtabletabletext.h>

class DelegateHighligtableTreeText : public DelegateHighligtableTableText
{
    Q_OBJECT
public:
    DelegateHighligtableTreeText(QObject *parent = nullptr):
        DelegateHighligtableTableText(parent)
    {Q_UNUSED(parent)}
    ~DelegateHighligtableTreeText(){}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        painter->save();
        drawHighlight(painter, option, index);

        painter->setPen(QColor(Qt::lightGray).lighter(100));

//        if (index.column() != 0 ) {
            painter->drawLine(QLine(option.rect.x()-1, option.rect.y(),
                                    option.rect.x()-1, option.rect.y()+option.rect.height()));
            painter->drawLine(QLine(option.rect.x()-1,
                                    option.rect.y()+option.rect.height()-1,
                                    option.rect.x()-1+option.rect.width(),
                                    option.rect.y()+option.rect.height()-1));
//        }
        painter->restore();
        QStyledItemDelegate::paint(painter, option, index);
    }
};

#endif // DELEGATEHIGHLIGTABLTREETEXT_H
