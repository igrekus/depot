#ifndef DELEGATEHIGHLIGTABLTREETEXT_H
#define DELEGATEHIGHLIGTABLTREETEXT_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QSortFilterProxyModel>
#include <QStyle>

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

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        painter->save();
//        painter->setClipRect(option.rect);

//        const QStyle *proxy = option.widget->style()->proxy();

//        // draw the background
//        QStyleOptionViewItem opt = option;
//        proxy->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter);

////        // draw the icon
////        QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
////        QIcon::Mode mode = QIcon::Normal;
////        if (!(vopt->state & QStyle::State_Enabled))
////            mode = QIcon::Disabled;
////        else if (vopt->state & QStyle::State_Selected)
////            mode = QIcon::Selected;
////        QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
////        vopt->icon.paint(p, iconRect, vopt->decorationAlignment, mode, state);

//        // draw the text
//        QRect textRect = proxy->subElementRect(QStyle::SE_ItemViewItemText, &opt);
//        if (!opt.text.isEmpty()) {
//            QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled
//                                      ? QPalette::Normal : QPalette::Disabled;
//            if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
//                cg = QPalette::Inactive;
//            if (opt.state & QStyle::State_Selected) {
//                painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
//            } else {
//                painter->setPen(opt.palette.color(cg, QPalette::Text));
//            }
//            proxy->drawItemText(painter, textRect, 0, opt.palette, true, opt.text);
//        }

        drawHighlight(painter, option, index);
        painter->setPen(QColor(Qt::lightGray).lighter(100));
//        if (index.row() == 0 || !index.parent().isValid()) {
//            painter->drawLine(QLine(option.rect.x()-1,
//                                    option.rect.y(),
//                                    option.rect.x()-1+option.rect.width(),
//                                    option.rect.y()));
//        }
        painter->drawLine(QLine(option.rect.x()-1, option.rect.y(),
                                option.rect.x()-1, option.rect.y()+option.rect.height()-1));
        painter->drawLine(QLine(option.rect.x()-1,
                                option.rect.y()+option.rect.height()-1,
                                option.rect.x()-1+option.rect.width(),
                                option.rect.y()+option.rect.height()-1));
        painter->restore();
        QStyledItemDelegate::paint(painter, option, index);
    }
};

#endif // DELEGATEHIGHLIGTABLTREETEXT_H
