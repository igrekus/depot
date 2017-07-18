#ifndef DELEGATEHIGHLIGTABLTREETEXT_H
#define DELEGATEHIGHLIGTABLTREETEXT_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QSortFilterProxyModel>
#include <QStyle>
#include <QStyledItemDelegate>

#include <constants.h>
#include <delegatehighligtabletabletext.h>

class DelegateHighligtableTreeText : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DelegateHighligtableTreeText(QObject *parent = nullptr):
        QStyledItemDelegate(parent)
    {Q_UNUSED(parent)}
    ~DelegateHighligtableTreeText(){}

    void drawHighlight(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
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

//    void QCommonStyle::drawControl(ControlElement element, const QStyleOption *opt,
//                                   QPainter *p, const QWidget *widget) const
//    case CE_ItemViewItem:
//        if (const QStyleOptionViewItem *vopt = qstyleoption_cast<const QStyleOptionViewItem *>(opt)) {
//            p->save();
//            p->setClipRect(opt->rect);

//            QRect checkRect = proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt, widget);
//            QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
//            QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);

//            // draw the background
//            proxy()->drawPrimitive(PE_PanelItemViewItem, opt, p, widget);

//            // draw the check mark
//            if (vopt->features & QStyleOptionViewItem::HasCheckIndicator) {
//                QStyleOptionViewItem option(*vopt);
//                option.rect = checkRect;
//                option.state = option.state & ~QStyle::State_HasFocus;

//                switch (vopt->checkState) {
//                case Qt::Unchecked:
//                    option.state |= QStyle::State_Off;
//                    break;
//                case Qt::PartiallyChecked:
//                    option.state |= QStyle::State_NoChange;
//                    break;
//                case Qt::Checked:
//                    option.state |= QStyle::State_On;
//                    break;
//                }
//                proxy()->drawPrimitive(QStyle::PE_IndicatorViewItemCheck, &option, p, widget);
//            }

//            // draw the icon
//            QIcon::Mode mode = QIcon::Normal;
//            if (!(vopt->state & QStyle::State_Enabled))
//                mode = QIcon::Disabled;
//            else if (vopt->state & QStyle::State_Selected)
//                mode = QIcon::Selected;
//            QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
//            vopt->icon.paint(p, iconRect, vopt->decorationAlignment, mode, state);

//            // draw the text
//            if (!vopt->text.isEmpty()) {
//                QPalette::ColorGroup cg = vopt->state & QStyle::State_Enabled
//                                      ? QPalette::Normal : QPalette::Disabled;
//                if (cg == QPalette::Normal && !(vopt->state & QStyle::State_Active))
//                    cg = QPalette::Inactive;

//                if (vopt->state & QStyle::State_Selected) {
//                    p->setPen(vopt->palette.color(cg, QPalette::HighlightedText));
//                } else {
//                    p->setPen(vopt->palette.color(cg, QPalette::Text));
//                }
//                if (vopt->state & QStyle::State_Editing) {
//                    p->setPen(vopt->palette.color(cg, QPalette::Text));
//                    p->drawRect(textRect.adjusted(0, 0, -1, -1));
//                }

//                d->viewItemDrawText(p, vopt, textRect);
//            }

//            // draw the focus rect
//             if (vopt->state & QStyle::State_HasFocus) {
//                QStyleOptionFocusRect o;
//                o.QStyleOption::operator=(*vopt);
//                o.rect = proxy()->subElementRect(SE_ItemViewItemFocusRect, vopt, widget);
//                o.state |= QStyle::State_KeyboardFocusChange;
//                o.state |= QStyle::State_Item;
//                QPalette::ColorGroup cg = (vopt->state & QStyle::State_Enabled)
//                              ? QPalette::Normal : QPalette::Disabled;
//                o.backgroundColor = vopt->palette.color(cg, (vopt->state & QStyle::State_Selected)
//                                             ? QPalette::Highlight : QPalette::Window);
//                proxy()->drawPrimitive(QStyle::PE_FrameFocusRect, &o, p, widget);
//            }

//             p->restore();
//        }
//        break;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {

        painter->save();

        painter->fillRect(option.rect, qvariant_cast<QBrush>(index.data(Constants::RoleBackground)));
        drawHighlight(painter, option, index);

        painter->setPen(QColor(Qt::lightGray).lighter(100));
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
