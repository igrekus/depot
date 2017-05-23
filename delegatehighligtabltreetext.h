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

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
//        p->save();
//        p->setClipRect(opt->rect);

//        option.widget->style()->proxy()
//        QRect checkRect = proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt, widget);
//        QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
//        QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);        // -------- !!!
//        // draw the background
//        proxy()->drawPrimitive(PE_PanelItemViewItem, opt, p, widget);

//        // draw the icon
//        QIcon::Mode mode = QIcon::Normal;
//        if (!(vopt->state & QStyle::State_Enabled))
//            mode = QIcon::Disabled;
//        else if (vopt->state & QStyle::State_Selected)
//            mode = QIcon::Selected;
//        QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
//        vopt->icon.paint(p, iconRect, vopt->decorationAlignment, mode, state);

//        // draw the focus rect
//         if (vopt->state & QStyle::State_HasFocus) {
//            QStyleOptionFocusRect o;
//            o.QStyleOption::operator=(*vopt);
//            o.rect = proxy()->subElementRect(SE_ItemViewItemFocusRect, vopt, widget);
//            o.state |= QStyle::State_KeyboardFocusChange;
//            o.state |= QStyle::State_Item;
//            QPalette::ColorGroup cg = (vopt->state & QStyle::State_Enabled)
//                          ? QPalette::Normal : QPalette::Disabled;
//            o.backgroundColor = vopt->palette.color(cg, (vopt->state & QStyle::State_Selected)
//                                         ? QPalette::Highlight : QPalette::Window);
//            proxy()->drawPrimitive(QStyle::PE_FrameFocusRect, &o, p, widget);
//        }

//         p->restore();


        painter->save();
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
